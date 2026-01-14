#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "Amp.h"
#include "AbyssAmp.h"
#include "NoiseReduction.h"
#include "RingBuffer.h"

//==============================================================================
class AbyssAudioProcessor final : public juce::AudioProcessor,
                                      public juce::AudioProcessorValueTreeState::Listener
{
  public:
    //==============================================================================
    AbyssAudioProcessor();
    ~AbyssAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    // AudioProcessorValueTreeState::Listener
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::AudioProcessorValueTreeState mParameters;
    std::unique_ptr<RingBuffer> mRingBuffer;

  private:
    void loadAllModels();
    
    static constexpr int kNumModels = 2; // Update when you add more models
    std::array<std::unique_ptr<Amp>, kNumModels> mModels;
    std::atomic<int> mActiveModelIndex{0};


    NoiseReduction mNoiseReduction;

    juce::AudioParameterFloat *mVolumeParameter;
    juce::AudioParameterFloat *mGainParameter;
    juce::AudioParameterFloat *mDenoiserParameter;
    juce::AudioParameterBool *mDenoiserActiveParameter;

    juce::AudioParameterChoice *mLoadedModelParameter;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AbyssAudioProcessor)
};
