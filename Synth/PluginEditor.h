#pragma once

#include "CustomLookAndFeel/FontAudioLookAndFeel.h"
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

    FontAudioLookAndFeel fontAudioLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessorEditor)
};
