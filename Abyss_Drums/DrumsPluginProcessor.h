#pragma once

#include <JuceHeader.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "DrumSampler.h"




//==============================================================================
class DrumsAudioProcessor final : public juce::AudioProcessor
{
  public:
    //==============================================================================
    DrumsAudioProcessor();
    ~DrumsAudioProcessor() override;

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

    void playPreviewSample(int drumType, int index);
    void loadDrumSample(int drumType, int index);
    void clearSample();
    int mDrumType{-1};
    int mDrumIndex{-1};

  private:
    //==============================================================================

    juce::WavAudioFormat wavFormat;
    DrumSampler loadedDrumSamples;
    std::unique_ptr<juce::AudioFormatReaderSource> audioFormatReaderSource;
    std::unique_ptr<juce::ResamplingAudioSource>   previewResampler;
    juce::SpinLock previewLock;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumsAudioProcessor)
};
