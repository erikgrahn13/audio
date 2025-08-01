#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
      mParameters(
          *this, nullptr, juce::Identifier("Parameters"),
          {std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"gain", 1}, "Gain", -48.0f, 0.0f, 0.0f),
           std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"loop", 1}, "Loop", false),
           std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"play", 1}, "Play", false),
           std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"stop", 1}, "Stop", false)

          })

{
    audioFormatManager.registerBasicFormats();

    gainParameter = dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("gain"));
    loopParameter = dynamic_cast<juce::AudioParameterBool *>(mParameters.getParameter("loop"));
    playParameter = dynamic_cast<juce::AudioParameterBool *>(mParameters.getParameter("play"));
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    previousGain = *gainParameter;
    transportSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
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

bool AudioPluginAudioProcessor::loadFile(juce::File &file)
{
    auto *reader = audioFormatManager.createReaderFor(file);
    if (reader != nullptr)
    {
        transportSource.stop();
        playParameter->setValueNotifyingHost(false);

        auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        audioFormatReaderSource.reset(newSource.release());

        return true;
    }
    return false;
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    buffer.clear();

    if (audioFormatReaderSource)
    {
        audioFormatReaderSource->setLooping(loopParameter->get());
    }

    auto currentGain = gainParameter->get();
    if (juce::approximatelyEqual(currentGain, previousGain))
    {
        buffer.applyGain(currentGain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }

    transportSource.setGain(juce::Decibels::decibelsToGain(currentGain));
    transportSource.getNextAudioBlock(juce::AudioSourceChannelInfo(buffer));

    if (playParameter->get())
    {
        transportSource.start();
    }
    else
    {
        transportSource.stop();
        transportSource.setPosition(0.0);
        buffer.clear();
    }

    if (transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds())
    {
        transportSource.stop();
        transportSource.setPosition(0.0);
        buffer.clear();
        playParameter->setValueNotifyingHost(false);
    }

    juce::ignoreUnused(midiMessages);

    // juce::ScopedNoDenormals noDenormals;
    // auto totalNumInputChannels = getTotalNumInputChannels();
    // auto totalNumOutputChannels = getTotalNumOutputChannels();

    // // In case we have more outputs than inputs, this code clears any output
    // // channels that didn't contain input data, (because these aren't
    // // guaranteed to be empty - they may contain garbage).
    // // This is here to avoid people getting screaming feedback
    // // when they first compile a plugin, but obviously you don't need to keep
    // // this code if your algorithm always overwrites all the output channels.
    // for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //     buffer.clear(i, 0, buffer.getNumSamples());

    // // This is the place where you'd normally do the guts of your plugin's
    // // audio processing...
    // // Make sure to reset the state if your inner loop is processing
    // // the samples and the outer loop is handling the channels.
    // // Alternatively, you can process the samples with the channels
    // // interleaved by keeping the same state.
    // for (int channel = 0; channel < totalNumInputChannels; ++channel)
    // {
    //     auto *channelData = buffer.getWritePointer(channel);
    //     juce::ignoreUnused(channelData);
    //     // ..do something to the data...
    // }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your mParameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    copyXmlToBinary(*mParameters.copyState().createXml(), destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your mParameters from this memory block,
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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
