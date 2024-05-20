#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
class SynthAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    SynthAudioProcessor();
    ~SynthAudioProcessor() override;

    //==============================================================================
    void setFrequency (double frequency) {mFrequency = frequency;};
    double getFrequency () {return mFrequency;};
    void setAmplitude (double amplitude) {mAmplitude = amplitude;};
    double getAmplitude () {return mAmplitude;};
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

private:
    double mAmplitude;
    double mFrequency;
    double mAngle;
    double mSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAudioProcessor)
};
