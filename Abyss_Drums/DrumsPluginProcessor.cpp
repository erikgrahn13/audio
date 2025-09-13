#include "DrumsPluginProcessor.h"
#include "DrumsPluginEditor.h"
#include "KickSamples.h"
#include "SnareSamples.h"

//==============================================================================
DrumsAudioProcessor::DrumsAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{

}

DrumsAudioProcessor::~DrumsAudioProcessor()
{
}

//==============================================================================
const juce::String DrumsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DrumsAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DrumsAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DrumsAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DrumsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DrumsAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int DrumsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DrumsAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String DrumsAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void DrumsAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void DrumsAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    if(previewResampler)
    {
        previewResampler->prepareToPlay(samplesPerBlock, sampleRate);
    }

    loadedDrumSamples.clearVoices();

    for (int i = 0; i < 32; ++i)
    {
        loadedDrumSamples.addVoice(new juce::SamplerVoice());
    }
    loadedDrumSamples.setCurrentPlaybackSampleRate(sampleRate);
}

void DrumsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DrumsAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
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

void DrumsAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    juce::MidiBuffer filtered;
    for (const auto meta : midiMessages)
    {
        const auto &m = meta.getMessage();
        if (m.isNoteOff())
            continue;

        filtered.addEvent(m, meta.samplePosition);
    }
    midiMessages.swapWith(filtered);

    buffer.clear();

    if(previewResampler)
    {
        juce::SpinLock::ScopedTryLockType tl (previewLock);
        if (tl.isLocked())
        {
            juce::AudioSourceChannelInfo channelInfo(buffer);
            previewResampler->getNextAudioBlock(channelInfo);
        }
    }


    loadedDrumSamples.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

//==============================================================================
bool DrumsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *DrumsAudioProcessor::createEditor()
{
    return new DrumsAudioProcessorEditor(*this);
}

//==============================================================================
void DrumsAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);

    juce::XmlElement state("DrumState");
    state.setAttribute("DrumType", mDrumType);
    state.setAttribute("DrumIndex", mDrumIndex);
    copyXmlToBinary(state, destData);
}

void DrumsAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
    auto xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName("DrumState"))
        {
            const auto drumType = xmlState->getIntAttribute("DrumType");
            const auto drumIndex = xmlState->getIntAttribute("DrumIndex");

            loadDrumSample(drumType, drumIndex);
            mDrumType = drumType;
            mDrumIndex = drumIndex;
        }
    }
}

void DrumsAudioProcessor::playPreviewSample(int drumType, int index)
{
    int dataSize = 0;

    const char *name;
    const void *data;
    if (drumType == 0) // Kicks
    {
        name = KickSamples::namedResourceList[index];
        data = KickSamples::getNamedResource(name, dataSize);
    }
    else if (drumType == 1) // Snare
    {
        name = SnareSamples::namedResourceList[index];
        data = SnareSamples::getNamedResource(name, dataSize);
    }
    else
    {
        name = nullptr;
        data = nullptr;
    }

    if (data != nullptr && dataSize > 0)
    {
        auto *reader = wavFormat.createReaderFor(new juce::MemoryInputStream(data, (size_t)dataSize, false), true);

        if (reader != nullptr)
        {
            juce::BigInteger allNotes;
            allNotes.setRange(0, 128, true);
            
            audioFormatReaderSource.reset();
            audioFormatReaderSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            
            const juce::SpinLock::ScopedLockType g (previewLock);
            previewResampler.reset();
            previewResampler = std::make_unique<juce::ResamplingAudioSource>(audioFormatReaderSource.get(), false);
            previewResampler->setResamplingRatio(reader->sampleRate / getSampleRate());

            previewResampler->prepareToPlay(getBlockSize(), getSampleRate());
            audioFormatReaderSource->setNextReadPosition(0);
        }
    }
}

void DrumsAudioProcessor::loadDrumSample(int drumType, int index)
{

    int dataSize = 0;

    const char *name;
    const void *data;
    juce::BigInteger note;

    if (drumType == 0) // Kicks
    {
        note.setBit(36); // C1 is kick drums
        name = KickSamples::namedResourceList[index];
        data = KickSamples::getNamedResource(name, dataSize);
    }
    else if (drumType == 1) // Snare
    {
        note.setBit(38); // D1 is snare drums
        name = SnareSamples::namedResourceList[index];
        data = SnareSamples::getNamedResource(name, dataSize);
    }
    else
    {
        // TODO: handle Toms later, need to come up with a plain how to handle toms
        name = nullptr;
        data = nullptr;
    }

    if (data != nullptr && dataSize > 0)
    {
        auto *reader = wavFormat.createReaderFor(new juce::MemoryInputStream(data, (size_t)dataSize, false), true);

        if (reader != nullptr)
        {
            const double maxLenSecs = reader->lengthInSamples / reader->sampleRate;

            auto *sound =
                new juce::SamplerSound(name, *reader, note, note.getHighestBit(), 0.0, 0.003,
                                maxLenSecs); // name, reader, midiNoteRange, rootNote, attack, release, maxLength

            loadedDrumSamples.removeSound(0);
            loadedDrumSamples.addSound(sound);
            mDrumType = drumType;
            mDrumIndex = index;


            delete reader;
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new DrumsAudioProcessor();
}
