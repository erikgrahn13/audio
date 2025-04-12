#pragma once

#include <JuceHeader.h>
#include <juce_audio_processors/juce_audio_processors.h>

#pragma once

#include <JuceHeader.h>


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
    std::atomic<bool> shouldTrigger { false };
    std::atomic<int> previewSampleIndex { -1};
    std::atomic<bool> shouldLoadPreviewSample { false };

    void updatePreviewSample(int drumType, int index);

  private:
    //==============================================================================

    int previousSampleIndex { -1 };
    int previousDrumTypeIndex { -1 };
    bool toggleNote{false};
    // juce::SmoothedValue<float> gain;
    juce::WavAudioFormat wavFormat;
    juce::Synthesiser previewSynth;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> audioFormatReaderSource;
    std::vector<juce::AudioSampleBuffer> drumSamples;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumsAudioProcessor)
};
