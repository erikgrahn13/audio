#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include <Biquad.h>

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
  public:
    using FilterTuple = std::tuple<Biquad, juce::RangedAudioParameter *, std::optional<juce::RangedAudioParameter *>,
                                   std::optional<juce::RangedAudioParameter *>>;

    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

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

    void parameterChanged(const juce::String &parameter, float newValue) override;
    void update();

    juce::AudioProcessorValueTreeState &getVTSParameters()
    {
        return mParameters;
    }
    std::vector<FilterTuple> getFilters()
    {
        return mFilters;
    }

    juce::AudioBuffer<float> mAudioBuffer;
    juce::AbstractFifo mRingBuffer{1};
    std::atomic<bool> nextFFTBlockReady{false};

  private:
    juce::AudioProcessorValueTreeState mParameters;
    juce::AudioParameterFloat *mHPF_Freq;
    std::map<Biquad, std::vector<juce::RangedAudioParameter *>> mTest;

    std::vector<FilterTuple> mFilters;

    std::atomic<bool> requiresUpdate{true};
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
