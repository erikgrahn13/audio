#pragma once

#include "PluginProcessor.h"

//==============================================================================
class SynthAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit SynthAudioProcessorEditor(SynthAudioProcessor &);
    ~SynthAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    SynthAudioProcessor &processorRef;
    juce::MidiKeyboardComponent keyboardComponent;
    juce::ToggleButton sineButton;
    juce::ToggleButton squareButton;
    juce::ToggleButton sawButton;
    juce::ToggleButton triangleButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessorEditor)
};
