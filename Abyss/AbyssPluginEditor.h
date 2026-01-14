#pragma once

#include <JuceHeader.h>
#include "AbyssPluginProcessor.h"
#include "CustomLookAndFeel/CustomStandaloneFilterWindow.h"
#include "CustomLookAndFeel/FontWebLookAndFeel.h"
#include "CustomLookAndFeel/DeathMetalLookAndFeel.h"


//==============================================================================
class AbyssAudioProcessorEditor final : public juce::AudioProcessorEditor, juce::Timer
{
  public:
    explicit AbyssAudioProcessorEditor(AbyssAudioProcessor &);
    ~AbyssAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    void timerCallback() override;

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AbyssAudioProcessor &processorRef;

    FontWebLookAndFeel fontWebLookAndFeel;
    DeathMetalLookAndFeel deathMetalLookAndFeel;

    juce::TextButton settingsButton;
    juce::Label titleLabel;
    juce::Rectangle<int> titleLabelBounds;
    juce::Image backgroundImage;
    juce::Rectangle<float> imageArea;

    juce::GroupComponent frame;
    juce::GroupComponent modelFrame;

    juce::Label volumeLabel;
    juce::Slider volumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mVolumeAttachment;

    juce::Label gainLabel;
    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachment;

    juce::Label denoiserLabel;
    juce::ToggleButton mDenoiserActiveButton;
    juce::Slider denoiserSlider;
    
    juce::TextButton modelButtonPrevious;
    juce::TextButton modelButtonNext;
    juce::Label loadedModelLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDenoiserAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mDenoiserActiveAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AbyssAudioProcessorEditor)
};
