#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "DrumSampler.h"
#include "DrumVoice.h"
#include "DrumSound.h"
#include "constants.h"

//==============================================================================
class DrumsAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    DrumsAudioProcessor();
    ~DrumsAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // Trigger a drum hit with given MIDI note and velocity
    void triggerNote(int midiNote, int velocity = 100)
    {
        float velocityNormalized = velocity / 127.0f;

        for(auto &drumSample : drumSamples)
        {
            if(midiNote == drumSample.first->getMidiNote())
            {
                drumSample.first->triggerNote(velocityNormalized);
                break;
            }
        }
        

        // if(midiNote == 36)
        //     kickSamples.triggerNote(midiNote, velocityNormalized);
        // else if(midiNote == 38)
        //     snareSamples.triggerNote(midiNote, velocityNormalized);
        // else if(midiNote == 46)
        //     hihatOpenSamples.triggerNote(midiNote, velocityNormalized);
        // else if(midiNote == 42)
        //     hihatClosedSamples.triggerNote(midiNote, velocityNormalized);
        // else if(midiNote == 48)
        //     tom1Samples.triggerNote(midiNote, velocityNormalized);
    }
    
    // Channel names for the 16-channel drum kit
    // static constexpr int numChannels = 16;
    // static constexpr const char* channelNames[numChannels] = {
    //     "AmbL", "AmbR", "Hihat", "KdrumL", "KdrumR",
    //     "OHL", "OHR", "RideL", "RideR", "Snare_bottom",
    //     "Snare_top", "Tom1", "Tom2", "Tom3", "Tom4", "Trigger"
    // };
    
    // Get current channel levels for metering
    void getChannelLevels(float* levels, int numLevels) const
    {
        for (int i = 0; i < numLevels && i < numChannels; ++i)
        {
            levels[i] = channelLevels[i].load();
        }
    }
    
    juce::AudioProcessorValueTreeState mParameters;
    std::array<std::atomic<float>, numChannels> channelLevels;


private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    juce::AudioFormatManager audioFormatManager;
    
    // Channel volume parameters (one for each of the 16 channels)
    std::array<juce::AudioParameterFloat*, numChannels> channelVolumes;
    
    
    struct ChannelParams {
        juce::AudioParameterFloat* volume;
        juce::AudioParameterBool* mute;
        juce::AudioParameterBool* solo;
    };
    std::array<ChannelParams, numChannels> channelParams;
    
    // Channel levels for metering (atomic for thread-safe access from UI)
    
    void loadDrumSamples(DrumSampler &drumSamples, const juce::String& path);

    // juce::SpinLock previewLock;
    // DrumSampler kickSamples{36};
    // DrumSampler snareSamples{38};
    // DrumSampler hihatOpenSamplπes{46};
    // DrumSampler hihatClosedSamples{42};
    // DrumSampler tom1Samples{48};
    std::vector<std::pair<std::unique_ptr<DrumSampler>, std::string>> drumSamples;
    // std::unique_ptr<juce::ResamplingAudioSource>   previewResampler;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrumsAudioProcessor)
};
