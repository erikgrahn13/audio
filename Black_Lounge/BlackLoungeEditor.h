#pragma once

#include "BlackLoungeProcessor.h"
#include "CustomLookAndFeel/CustomStandaloneFilterWindow.h"
#include "CustomLookAndFeel/DeathMetalLookAndFeel.h"
#include "CustomLookAndFeel/FontWebLookAndFeel.h"
#include <JuceHeader.h>

//==============================================================================
class BlackLoungeAudioProcessorEditor final : public juce::AudioProcessorEditor
{
  public:
    explicit BlackLoungeAudioProcessorEditor(BlackLoungeAudioProcessor &);
    ~BlackLoungeAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BlackLoungeAudioProcessor &processorRef;
    juce::TextButton button;
    juce::TextButton analyzeButton;
    juce::Label volumeLabel;
    juce::Slider volumeSlider;
    juce::Label thresholdLabel;
    juce::Slider thresholdSlider;
    juce::Label gainLabel;
    juce::Slider gainSlider;
    juce::TabbedComponent mainScreens{juce::TabbedButtonBar::TabsAtBottom};
    FontWebLookAndFeel fontWebLookAndFeel;
    DeathMetalLookAndFeel fontDeathMetalLookAndFeel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mVolumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mThresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mAnalyzeAttachment;

    juce::Slider testSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mTestAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlackLoungeAudioProcessorEditor)
};
