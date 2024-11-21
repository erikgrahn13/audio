#pragma once

#include "Amp.h"
#include "BlackLoungeAmp.h"
#include "NAM/dsp.h"
#include "NoiseReduction.h"
#include <JuceHeader.h>
#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
class BlackLoungeAudioProcessor final : public juce::AudioProcessor
{
  public:
    //==============================================================================
    BlackLoungeAudioProcessor();
    ~BlackLoungeAudioProcessor() override;

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

    juce::AudioProcessorValueTreeState &getVTSParameters()
    {
        return mParameters;
    }

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

  private:
    std::unique_ptr<Amp> mBlackLoungeAmp;
    juce::AudioProcessorValueTreeState mParameters;
    juce::AudioParameterFloat *mVolumeParameter;
    juce::AudioParameterFloat *mThresholdParameter;
    juce::AudioParameterFloat *mGainParameter;
    juce::AudioParameterBool *mNoiseGateActiveParameter;

    juce::AudioParameterBool *mAnalyzeParameter;

    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>> mChain;
    NoiseReduction mNoiseReduction;

    juce::AudioParameterFloat *mTestParameter;

    juce::AudioBuffer<float> analysisBuffer;
    int analysisBufferPosition{0};
    bool bufferFilled{false};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlackLoungeAudioProcessor)
};
