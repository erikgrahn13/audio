#pragma once

#include "PluginProcessor.h"

//==============================================================================
class SynthAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit SynthAudioProcessorEditor (SynthAudioProcessor&);
    ~SynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthAudioProcessor& processorRef;
    juce::Slider freqKnob;
    juce::Label freqLabel;
    juce::Slider gainKnob;
    juce::Label gainLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAudioProcessorEditor)
};
