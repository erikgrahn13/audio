#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <JuceHeader.h>

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
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

    bool loadFile(juce::File &file);
    juce::AudioTransportSource &getTransportSource() { return transportSource; }
    juce::AudioFormatReaderSource &getAudioFormatReaderSource() { return *audioFormatReaderSource; }

private:
    juce::AudioTransportSource transportSource;
    juce::AudioFormatManager audioFormatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> audioFormatReaderSource;
    bool mShouldLoop{false};

    juce::AudioParameterFloat* volume;
    float previousGain;
    juce::AudioParameterFloat* gainParameter;
    juce::AudioParameterBool* loopParameter;
    juce::AudioParameterBool* playParameter;
    juce::AudioParameterBool* stopParameter;

    juce::AudioProcessorValueTreeState parameters;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
