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
    juce::Label titleLabel;
    juce::TextButton settingsButton;
    juce::TextButton analyzeButton;
    juce::TextButton mDenoiserActiveButton;
    juce::Label volumeLabel;
    juce::Slider volumeSlider;
    juce::Label denoiserLabel;
    juce::Slider denoiserSlider;
    juce::Label gainLabel;
    juce::Slider gainSlider;
    FontWebLookAndFeel fontWebLookAndFeel;
    DeathMetalLookAndFeel fontDeathMetalLookAndFeel;

    juce::Image backgroundImage;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mVolumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDenoiserAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mAnalyzeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mDenoiserActiveAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlackLoungeAudioProcessorEditor)
};
