#pragma once

#include "Amp.h"
#include "BlackLoungeAmp.h"
#include "NAM/dsp.h"
#include "NoiseReduction.h"
#include "RingBuffer.h"
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

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::AudioProcessorValueTreeState mParameters;
    std::unique_ptr<RingBuffer> mRingBuffer;

  private:
    std::unique_ptr<juce::AudioBuffer<float>> mAudioBuffer;
    std::unique_ptr<Amp> mBlackLoungeAmp;
    juce::AudioParameterFloat *mVolumeParameter;
    juce::AudioParameterFloat *mGainParameter;

    NoiseReduction mNoiseReduction;

    juce::AudioParameterFloat *mDenoiserParameter;
    juce::AudioParameterBool *mDenoiserActiveParameter;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlackLoungeAudioProcessor)
};
