#include "AbyssPluginProcessor.h"
#include "AbyssPluginEditor.h"

//==============================================================================
AbyssAudioProcessor::AbyssAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
      mParameters(*this, nullptr, juce::Identifier("Parameters"), createParameters())
{
    mVolumeParameter = dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("volume"));
    mGainParameter = dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("gain"));
    mDenoiserActiveParameter = dynamic_cast<juce::AudioParameterBool *>(mParameters.getParameter("denoiserActive"));
    mDenoiserParameter = dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("denoiser"));
    mLoadedModelParameter = dynamic_cast<juce::AudioParameterChoice *>(mParameters.getParameter("loadedModel"));

    mRingBuffer = std::make_unique<RingBuffer>(4096 * 4);

    
    // Pre-load all models (happens once at startup, not real-time)
    loadAllModels();
    
    // Set initial active model
    mActiveModelIndex.store(mLoadedModelParameter->getIndex());
    
    // Listen for parameter changes
    mParameters.addParameterListener("loadedModel", this);
}

AbyssAudioProcessor::~AbyssAudioProcessor()
{
}

//==============================================================================
const juce::String AbyssAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AbyssAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AbyssAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AbyssAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AbyssAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AbyssAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int AbyssAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AbyssAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AbyssAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return "default";
}

void AbyssAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AbyssAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumInputChannels());
    spec.sampleRate = sampleRate;
    mNoiseReduction.prepare(spec);
}

void AbyssAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
     // spare memory, etc.
}

bool AbyssAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void AbyssAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    // auto totalNumInputChannels = getTotalNumInputChannels();
    // auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    mRingBuffer->addToFifo(buffer.getWritePointer(0), buffer.getNumSamples());

    
    if (mDenoiserActiveParameter->get())
    {
        auto threshold = mDenoiserParameter->get();
        mNoiseReduction.setThreshold(threshold);
        mNoiseReduction.process(buffer);
    }

    auto volume = juce::Decibels::decibelsToGain(mVolumeParameter->get());
    
    // Get the currently active model (real-time safe atomic read)
    int activeIndex = mActiveModelIndex.load();
    if (auto* activeModel = mModels[activeIndex].get())
    {
#if defined NDEBUG
        auto gain = juce::Decibels::decibelsToGain(mGainParameter->get());
        auto *input = buffer.getWritePointer(0);
        auto *output = buffer.getWritePointer(0);

        buffer.applyGain(gain);
        activeModel->process(input, output, buffer.getNumSamples());
#endif
    }


    // for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        // buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    buffer.applyGain(volume);
    if (buffer.getNumChannels() > 1)
        buffer.copyFrom(1, 0, buffer.getReadPointer(0), buffer.getNumSamples());
}

//==============================================================================
bool AbyssAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AbyssAudioProcessor::createEditor()
{
    return new AbyssAudioProcessorEditor(*this);
    // return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void AbyssAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    copyXmlToBinary(*mParameters.copyState().createXml(), destData);
}

void AbyssAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(mParameters.state.getType()))
        {
            mParameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

void AbyssAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "loadedModel")
    {
        int modelIndex = static_cast<int>(newValue * (mLoadedModelParameter->choices.size() - 1) + 0.5f);
        // Real-time safe: just atomically switch the active model index
        mActiveModelIndex.store(modelIndex);
    }
}

void AbyssAudioProcessor::loadAllModels()
{
    // Load all models at startup (not real-time, so allocations are fine)
    mModels[0] = std::make_unique<Amp>(AbyssAmp::abyss1_nam, AbyssAmp::abyss1_namSize);
    
    // Load second model (using abyss1 as placeholder until you add abyss2)
    mModels[1] = std::make_unique<Amp>(AbyssAmp::abyss1_nam, AbyssAmp::abyss1_namSize);
    // When you have abyss2:
    // mModels[1] = std::make_unique<Amp>(AbyssAmp::abyss2_nam, AbyssAmp::abyss2_namSize);
    
    // Add more models here as needed (update kNumModels constant in header)
}

juce::AudioProcessorValueTreeState::ParameterLayout AbyssAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"gain", 1}, "gain", -80.f, 0.f, -20.f));
    // layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"bypass", 1}, "bypass", true));
    layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterID{"volume", 1}, "Volume", -10.f, 20.f, 18.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterID{"gain", 1}, "Gain", -20.f, 20.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterBool>(ParameterID{"denoiserActive", 1}, "DenoiserActive", true));
    layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterID{"denoiser", 1}, "Denoiser", -140.f, 0.f, -140.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>(ParameterID{"loadedModel", 1}, "LoadedModel", juce::StringArray{"ABYSS1", "ABYSS2"}, 0));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AbyssAudioProcessor();
}
