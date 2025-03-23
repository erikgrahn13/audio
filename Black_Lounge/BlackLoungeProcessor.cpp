#include "BlackLoungeProcessor.h"
#include "BlackLoungeEditor.h"
#include "architecture.h"

//==============================================================================
BlackLoungeAudioProcessor::BlackLoungeAudioProcessor()
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
    mThresholdParameter = dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("threshold"));
    mGainParameter = dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("gain"));

    mBlackLoungeAmp = std::make_unique<Amp>(BlackLoungeAmp::ironmaster_nam, BlackLoungeAmp::ironmaster_namSize);
}

BlackLoungeAudioProcessor::~BlackLoungeAudioProcessor()
{
}

//==============================================================================
const juce::String BlackLoungeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BlackLoungeAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool BlackLoungeAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool BlackLoungeAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double BlackLoungeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BlackLoungeAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int BlackLoungeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BlackLoungeAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String BlackLoungeAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void BlackLoungeAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void BlackLoungeAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void BlackLoungeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool BlackLoungeAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
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

void BlackLoungeAudioProcessor::doDualMono(juce::AudioBuffer<float> &mainBuffer, float **input)
{
    auto channelDataLeft = mainBuffer.getWritePointer(0);
    auto channelDataRight = mainBuffer.getWritePointer(1);

    for (int sample = 0; sample < mainBuffer.getNumSamples(); ++sample)
    {
        channelDataRight[sample] = input[0][sample];
        channelDataLeft[sample] = input[0][sample];
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout BlackLoungeAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("volume", "Volume", -10.f, 10.f, 0.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("threshold", "Threshold", -100.f, 0.f, -80.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("gain", "Gain", -20.f, 20.f, 0.f));

    return {parameters.begin(), parameters.end()};
}

void BlackLoungeAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);
    std::ignore = buffer;

    auto gain = juce::Decibels::decibelsToGain(mGainParameter->get());
    // auto threshold = juce::Decibels::decibelsToGain(mThresholdParameter->get());
    auto volume = juce::Decibels::decibelsToGain(mVolumeParameter->get());

    if (mBlackLoungeAmp)
    {
        buffer.applyGain(gain);
        mBlackLoungeAmp->process(buffer.getWritePointer(0), buffer.getWritePointer(0), buffer.getNumSamples());

        buffer.applyGain(volume);

        if (buffer.getNumChannels() > 1)
            buffer.copyFrom(1, 0, buffer.getReadPointer(0), buffer.getNumSamples());
    }
}

//==============================================================================
bool BlackLoungeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *BlackLoungeAudioProcessor::createEditor()
{
    auto *topLevel = juce::TopLevelWindow::getTopLevelWindow(0);
    if (topLevel)
    {
        topLevel->setUsingNativeTitleBar(true);
    }

    return new BlackLoungeAudioProcessorEditor(*this);
}

//==============================================================================
void BlackLoungeAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void BlackLoungeAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new BlackLoungeAudioProcessor();
}
