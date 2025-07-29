#pragma once

#include "CustomLookAndFeel/DeathMetalLookAndFeel.h"
#include "EQView.h"
#include "PluginProcessor.h"
#include "SliderGroup.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                              juce::AudioProcessorValueTreeState::Listener
{
  public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    void parameterChanged(const juce::String &parameterID, float newValue) override;

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor &processorRef;
    EQView mEQView;
    SliderGroup mHPFSliderGroup;
    SliderGroup mLowShelfSliderGroup;
    SliderGroup mLowMidSliderGroup;
    SliderGroup mHighMidSliderGroup;
    SliderGroup mHighShelfSliderGroup;
    SliderGroup mLPFSliderGroup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
