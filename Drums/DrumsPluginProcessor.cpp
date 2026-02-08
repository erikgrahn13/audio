#include "DrumsPluginProcessor.h"
#include "DrumsPluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
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
        juce::String paramID = juce::String(channelNames[i]) + "Volume";
        channelVolumes[i] = dynamic_cast<juce::AudioParameterFloat*>(mParameters.getParameter(paramID));
    }
    
    // Load kick samples
    loadKickSamples();
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
void AudioPluginAudioProcessor::loadKickSamples()
{
    juce::File samplesDir("/home/erik/Downloads/MuldjordKit3/KdrumR/samples");
    
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
                                           36,  // MIDI note 36 = kick (C1)
                                           reader->sampleRate,
                                           velocityRanges[layer][0],
                                           velocityRanges[layer][1]);
                
                kickSamples.addSound(sound);
                
                DBG("  Loaded: " + file.getFileName() + " - Layer " + juce::String(layer) + 
                    " (vel " + juce::String(velocityRanges[layer][0]) + "-" + 
                    juce::String(velocityRanges[layer][1]) + ")");
            }
        }
    }
    
    DBG("Total kick sounds loaded: " + juce::String(kickSamples.getNumSounds()));
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);

    if(previewResampler)
    {
        previewResampler->prepareToPlay(samplesPerBlock, sampleRate);
    }

    kickSamples.clearVoices();

    for(int i = 0; i < 32; ++i)
    {
        kickSamples.addVoice(new DrumVoice(channelVolumes, channelLevels));
    }
    kickSamples.setCurrentPlaybackSampleRate(sampleRate);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
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

    buffer.clear();

    kickSamples.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Create volume parameter for each of the 16 channels
    for (int i = 0; i < numChannels; ++i)
    {
        juce::String paramID = juce::String(channelNames[i]) + "Volume";
        juce::String paramName = juce::String(channelNames[i]) + " Vol";
        
        // Default: main kick channels (KdrumL, KdrumR) at -6dB, others at -80dB (muted)
        float defaultValue = (i == 3 || i == 4) ? -6.0f : -80.0f;
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{paramID, 1}, 
            paramName, 
            -80.f, 12.f, defaultValue));
    }

    return layout;
}
