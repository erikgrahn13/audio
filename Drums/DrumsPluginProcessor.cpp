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
    // std::unique_ptr<AudioFormatReader> audioReader (wavFormat.createReaderFor (createAssetInputStream ("cello.wav").release(), true));
    // auto test = wavFormat.createReaderFor(std::unique_ptr<juce::MemoryInputStream>(DrumSamples::reverb_ir_wav, DrumSamples::reverb_ir_wavSize, false), true);
    // auto memoryInputStream  = std::unique_ptr<juce::MemoryInputStream>(DrumSamples::reverb_ir_wav, DrumSamples::reverb_ir_wavSize, false);
    // auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(DrumSamples::reverb_ir_wav, DrumSamples::reverb_ir_wavSize, false);
    // auto *reader = wavFormat.createReaderFor(memoryInputStream.get(), true);

    // for(int i = 0; i < DrumSamples::namedResourceListSize; ++i)
    // {
    //     int dataSizeInBytes;

    //     auto *sampleData = DrumSamples::getNamedResource(DrumSamples::namedResourceList[i], dataSizeInBytes);

    //     auto* reader = wavFormat.createReaderFor(
    //         new juce::MemoryInputStream(sampleData, dataSizeInBytes, false),
    //         true); // "true" = JUCE will delete it

    //         juce::AudioSampleBuffer buffer(reader->numChannels, static_cast<int>(reader->lengthInSamples));
    //         reader->read(&buffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);
    //         drumSamples.push_back(std::move(buffer));
    //         delete reader;
        
    // }
    
    // auto* reader = wavFormat.createReaderFor(
    //     new juce::MemoryInputStream(DrumSamples::reverb_ir_wav, DrumSamples::reverb_ir_wavSize, false),
    //     true); // "true" = JUCE will delete it




    // auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
    // transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    // audioFormatReaderSource.reset(newSource.release());
    // previewSynth.addVoice(new juce::SamplerVoice());


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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // juce::ignoreUnused(sampleRate, samplesPerBlock);
    // transportSource.prepareToPlay(samplesPerBlock, sampleRate);

    std::ignore = samplesPerBlock;
    previewSynth.clearVoices();

    for(int i = 0; i < 4; ++i)
    {
        previewSynth.addVoice(new SamplerVoice());
    }
    previewSynth.setCurrentPlaybackSampleRate(sampleRate);

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
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;

    // if (shouldTrigger.exchange(false)) {
    //     // if(!transportSource.isPlaying())
    //     // {
    //     //     transportSource.stop();
    //     // }
    //     transportSource.setPosition(0.0);
    //     transportSource.start();
    // }
    // if (shouldTrigger.exchange(false)) {
    //     if (!transportSource.isPlaying()) {
    //         transportSource.setPosition(0.0);
    //         transportSource.start();
    //     } else {
    //         transportSource.stop();
    //         transportSource.setPosition(0.0);
    //         transportSource.start(); // restart cleanly if already playing
    //     }
    // }

    // transportSource.getNextAudioBlock(juce::AudioSourceChannelInfo(buffer));
    // if (!transportSource.isPlaying() && transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds()) {
    //     transportSource.stop();
    // }

    previewSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
//     for (int i = 0; i < numSamples; ++i)
// {
//     float g = gain.getNextValue();
//     for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
//         outputBuffer.getWritePointer(channel, startSample)[i] *= g;
// }

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
}

void DrumsAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

void DrumsAudioProcessor::updatePreviewSample(int drumType, int index)
{
    // if(index != previewSampleIndex.load())
    // {
    //     const char* name = DrumSamples::namedResourceList[index];
    //     int dataSize = 0;
    //     const void* data = DrumSamples::getNamedResource(name, dataSize);
    
    //     if (data != nullptr && dataSize > 0)
    //     {
    //         auto* reader = wavFormat.createReaderFor(
    //             new juce::MemoryInputStream(data, dataSize, false),
    //             true);
    
    //         if (reader != nullptr)
    //         {
    //             auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
    
    //             transportSource.stop();
    //             transportSource.setSource(nullptr); // Clear existing source
    
    //             transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    
    //             // Keep the new source alive
    //             audioFormatReaderSource = std::move(newSource);
    //         }
    //     }
    //     previewSampleIndex.store(index);
    // }
    // shouldTrigger.store(true);

    if(drumType != previousDrumTypeIndex)
    {
        int dataSize = 0;

        const char* name;
        const void* data;
        if(drumType == 0) // Kicks
        {
            name =  KickSamples::namedResourceList[index];
            data = KickSamples::getNamedResource(name, dataSize);
        }
        else if(drumType == 1) // Snare
        {
            name =  SnareSamples::namedResourceList[index];
            data = SnareSamples::getNamedResource(name, dataSize);
        }
        else{
            name = nullptr;
            data = nullptr;
        }
        // const char* name = KickSamples::namedResourceList[index];
        // const void* data = KickSamples::getNamedResource(name, dataSize);

        if (data != nullptr && dataSize > 0)
        {
            auto* reader = wavFormat.createReaderFor(
                new juce::MemoryInputStream(data, (size_t) dataSize, false),
                true);

            if (reader != nullptr)
            {
                juce::BigInteger allNotes;
                allNotes.setRange(0, 128, true);

                auto* sound = new juce::SamplerSound(
                    name, *reader, allNotes, 60, 0.0, 0.0, 5.0); // name, reader, midiNoteRange, rootNote, attack, release, maxLength

                previewSynth.clearSounds(); // Only one preview sound at a time
                previewSynth.addSound(sound);


                delete reader;

                // Trigger MIDI note 60 (Middle C)
                // juce::MidiMessage on = juce::MidiMessage::noteOn(1, 60, (juce::uint8) 127);
            }
        }
        previousSampleIndex = index;
    }
    // previewSynth.allNotesOff(1, false);
    // previewSynth.getVoice(0)->stopNote(0.0, false);
    // previewSynth.noteOff(1, 60, 0.0f, false); // fade out previous voice cleanly
    previewSynth.noteOn(1, 60, 1.0f);

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new DrumsAudioProcessor();
}
