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
    juce::TextButton sineButton;
    juce::TextButton squareButton;
    juce::TextButton sawButton;
    juce::TextButton triangleButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> sineButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> squareButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> sawButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> triangleButtonAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessorEditor)
};
