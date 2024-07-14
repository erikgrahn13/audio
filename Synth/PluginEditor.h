#pragma once

#include "PluginProcessor.h"
#include "../utilities/FontAudioLookAndFeel.h"

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
    FontAudioLookAndFeel fontAudioLookAndFeel;

    juce::MidiKeyboardComponent keyboardComponent;
    juce::TextButton sineButton;
    juce::TextButton squareButton;
    juce::TextButton sawButton;
    juce::TextButton triangleButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessorEditor)
};
