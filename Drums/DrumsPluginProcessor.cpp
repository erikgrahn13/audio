#include "DrumsPluginProcessor.h"
#include "DrumsPluginEditor.h"
#include <filesystem>

//==============================================================================
DrumsAudioProcessor::DrumsAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    mParameters(*this, nullptr, juce::Identifier("Parameters"), createParameters())
{
    // Register audio formats (WAV, AIFF, etc.)
    audioFormatManager.registerBasicFormats();
    
    // Get channel volume parameters
    for (int i = 0; i < numChannels; ++i)
    {
        juce::String ParamID = juce::String(channelNames[i]) + "Volume";
        // juce::String volumeParamID = juce::String(channelNames[i]) + "Volume";
        // juce::String muteParamID = juce::String(channelNames[i]) + "Mute";
        // juce::String soloParamID = juce::String(channelNames[i]) + "Solo";

        // channelParams.at(i).volume = dynamic_cast<juce::AudioParameterFloat*>(mParameters.getParameter(volumeParamID));
        channelVolumes[i] = dynamic_cast<juce::AudioParameterFloat*>(mParameters.getParameter(ParamID));
    }
    
    drumSamples.emplace_back(std::make_unique<DrumSampler>(36), "/Users/erikgrahn/Downloads/MuldjordKit3/KdrumR/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(38), "/Users/erikgrahn/Downloads/MuldjordKit3/Snare/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(46), "/Users/erikgrahn/Downloads/MuldjordKit3/HihatOpen/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(42), "/Users/erikgrahn/Downloads/MuldjordKit3/HihatClosed/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(48), "/Users/erikgrahn/Downloads/MuldjordKit3/Tom1/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(47), "/Users/erikgrahn/Downloads/MuldjordKit3/Tom2/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(45), "/Users/erikgrahn/Downloads/MuldjordKit3/Tom3/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(41), "/Users/erikgrahn/Downloads/MuldjordKit3/Tom4/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(59), "/Users/erikgrahn/Downloads/MuldjordKit3/RideL/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(51), "/Users/erikgrahn/Downloads/MuldjordKit3/RideR/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(52), "/Users/erikgrahn/Downloads/MuldjordKit3/China/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(49), "/Users/erikgrahn/Downloads/MuldjordKit3/CrashL/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(57), "/Users/erikgrahn/Downloads/MuldjordKit3/CrashR/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(55), "/Users/erikgrahn/Downloads/MuldjordKit3/RideLBell/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(53), "/Users/erikgrahn/Downloads/MuldjordKit3/RideRBell/samples");
    drumSamples.emplace_back(std::make_unique<DrumSampler>(37), "/Users/erikgrahn/Downloads/MuldjordKit3/SnareRest/samples");

    for(auto & [sampler, path] : drumSamples)
    {
        loadDrumSamples(*sampler, path);
    }

    // Load kick samples
    // loadDrumSamples(kickSamples, "/Users/erikgrahn/Downloads/MuldjordKit3/KdrumR/samples", 36);
    // loadDrumSamples(snareSamples, "/Users/erikgrahn/Downloads/MuldjordKit3/Snare/samples", 38);
    // loadDrumSamples(hihatOpenSamples, "/Users/erikgrahn/Downloads/MuldjordKit3/HihatOpen/samples", 46);
    // loadDrumSamples(hihatClosedSamples, "/Users/erikgrahn/Downloads/MuldjordKit3/HihatClosed/samples", 42);
    // loadDrumSamples(tom1Samples, "/Users/erikgrahn/Downloads/MuldjordKit3/Tom1/samples", 48);
}

DrumsAudioProcessor::~DrumsAudioProcessor()
{
}

//==============================================================================
void DrumsAudioProcessor::loadDrumSamples(DrumSampler& drumSamples, const juce::String& path)
{
    juce::File samplesDir(path);
    
    if (!samplesDir.exists())
    {
        DBG("Kick samples directory not found: " + samplesDir.getFullPathName());
        return;
    }
    
    auto files = samplesDir.findChildFiles(juce::File::findFiles, false, "*.wav");
    
    // Sort by numeric part of filename
    struct FileComparator
    {
        static int compareElements(const juce::File& a, const juce::File& b)
        {
            auto getNumber = [](const juce::String& filename) {
                return filename.upToFirstOccurrenceOf("-", false, false).getIntValue();
            };
            int numA = getNumber(a.getFileName());
            int numB = getNumber(b.getFileName());
            return (numA < numB) ? -1 : (numA > numB) ? 1 : 0;
        }
    };
    
    FileComparator comparator;
    files.sort(comparator);
    
    DBG("Loading kick samples: Found " + juce::String(files.size()) + " files");
    
    // Define 5 velocity layers
    const int numLayers = 5;
    const int velocityRanges[numLayers][2] = {
        {0, 25},      // Very soft
        {26, 51},     // Soft
        {52, 76},     // Medium
        {77, 102},    // Hard
        {103, 127}    // Very hard
    };
    
    // Distribute samples across velocity layers
    const int totalSamples = files.size();
    const int samplesPerLayer = totalSamples / numLayers;
    const int remainder = totalSamples % numLayers;
    
    int sampleIndex = 0;
    
    // Load each kick sample as a DrumSound with velocity range
    for (int layer = 0; layer < numLayers; ++layer)
    {
        int samplesInThisLayer = samplesPerLayer + (layer < remainder ? 1 : 0);
        
        for (int i = 0; i < samplesInThisLayer && sampleIndex < totalSamples; ++i, ++sampleIndex)
        {
            const auto& file = files[sampleIndex];
            std::unique_ptr<juce::AudioFormatReader> reader(audioFormatManager.createReaderFor(file));
            
            if (reader != nullptr)
            {
                if (reader->numChannels < 16)
                {
                    DBG("  WARNING: Skipping " + file.getFileName() + " - only has " + 
                        juce::String(reader->numChannels) + " channels (need 16)");
                    continue;
                }
                
                // Keep all 16 channels
                juce::AudioBuffer<float> fullBuffer(16, (int)reader->lengthInSamples);
                
                // Read all channels from reader
                reader->read(&fullBuffer, 0, (int)reader->lengthInSamples, 0, true, true);
                
                // Create DrumSound with velocity range and add to synthesiser
                auto* sound = new DrumSound(file.getFileNameWithoutExtension(),
                                           std::move(fullBuffer),
                                           drumSamples.getMidiNote(),  // MIDI note 36 = kick (C1)
                                           reader->sampleRate,
                                           velocityRanges[layer][0],
                                           velocityRanges[layer][1]);
                
                drumSamples.addSound(sound);
                
                DBG("  Loaded: " + file.getFileName() + " - Layer " + juce::String(layer) + 
                    " (vel " + juce::String(velocityRanges[layer][0]) + "-" + 
                    juce::String(velocityRanges[layer][1]) + ")");
            }
        }
    }
    
    DBG("Total kick sounds loaded: " + juce::String(drumSamples.getNumSounds()));
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DrumsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DrumsAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String DrumsAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void DrumsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void DrumsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);

    for(auto & drumSample : drumSamples)
    {
        drumSample.first->clearVoices();
    }

    // kickSamples.clearVoices();
    // snareSamples.clearVoices();
    // hihatOpenSamples.clearVoices();
    // hihatClosedSamples.clearVoices();
    // tom1Samples.clearVoices();

    for(int i = 0; i < 32; ++i)
    {
        for(auto & drumSample : drumSamples)
        {
            drumSample.first->addVoice(new DrumVoice(channelVolumes, channelLevels));
        }
        // kickSamples.addVoice(new DrumVoice(channelVolumes, channelLevels));
        // snareSamples.addVoice(new DrumVoice(channelVolumes, channelLevels));
        // hihatOpenSamples.addVoice(new DrumVoice(channelVolumes, channelLevels));
        // hihatClosedSamples.addVoice(new DrumVoice(channelVolumes, channelLevels));
        // tom1Samples.addVoice(new DrumVoice(channelVolumes, channelLevels));
    }

    for(auto & drumSample : drumSamples)
    {
        drumSample.first->setCurrentPlaybackSampleRate(sampleRate);
    }

    // kickSamples.setCurrentPlaybackSampleRate(sampleRate);
    // snareSamples.setCurrentPlaybackSampleRate(sampleRate);
    // hihatOpenSamples.setCurrentPlaybackSampleRate(sampleRate);
    // hihatClosedSamples.setCurrentPlaybackSampleRate(sampleRate);
    // tom1Samples.setCurrentPlaybackSampleRate(sampleRate);
}

void DrumsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DrumsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void DrumsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    juce::MidiBuffer filtered;
    for(const auto meta : midiMessages)
    {
        const auto &m = meta.getMessage();
        if(m.isNoteOff())
            continue;

        filtered.addEvent(m, meta.samplePosition);
    }
    midiMessages.swapWith(filtered);

    // Create 16-channel intermediate buffer for drum rendering
    juce::AudioBuffer<float> sixteenChannelBuffer(16, buffer.getNumSamples());
    sixteenChannelBuffer.clear();

    // Render all drum samples to 16-channel buffer
    for(auto & drumSample : drumSamples)
    {
        drumSample.first->renderNextBlock(sixteenChannelBuffer, midiMessages, 0, buffer.getNumSamples());
    }

    // Calculate RMS for each channel for metering
    for(int ch = 0; ch < numChannels; ++ch)
    {
        float rms = sixteenChannelBuffer.getRMSLevel(ch, 0, buffer.getNumSamples());
        channelLevels[ch].store(rms);
    }

    // Mix 16 channels to stereo output using channel volumes
    buffer.clear();
    for(int ch = 0; ch < numChannels; ++ch)
    {
        const float gain = juce::Decibels::decibelsToGain(channelVolumes[ch]->get());
        
        for(int i = 0; i < buffer.getNumSamples(); ++i)
        {
            const float sample = sixteenChannelBuffer.getSample(ch, i) * gain;
            buffer.addSample(0, i, sample);  // Left
            buffer.addSample(1, i, sample);  // Right
        }
    }

    // kickSamples.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    // snareSamples.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    // hihatOpenSamples.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    // hihatClosedSamples.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    // hihatClosedSamples.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool DrumsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DrumsAudioProcessor::createEditor()
{
    return new DrumsAudioProcessorEditor (*this);
}

//==============================================================================
void DrumsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void DrumsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DrumsAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout DrumsAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Create volume parameter for each of the 16 channels
    for (int i = 0; i < numChannels; ++i)
    {
        juce::String paramID = juce::String(channelNames[i]) + "Volume";
        juce::String paramName = juce::String(channelNames[i]) + " Vol";
        
        // Default: main kick channels (KdrumL, KdrumR) at -6dB, others at -80dB (muted)
        // float defaultValue = ( i == 4 || i == 2) ? -6.0f : -80.0f;
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{paramID, 1}, 
            paramName, 
            -80.f, 12.f, -6.f));
    }

    return layout;
}
