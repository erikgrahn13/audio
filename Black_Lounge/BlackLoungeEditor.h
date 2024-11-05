#pragma once

#include "AmpView.h"
#include "BlackLoungeProcessor.h"
#include "CustomLookAndFeel/CustomStandaloneFilterWindow.h"
#include "CustomLookAndFeel/FontWebLookAndFeel.h"
#include "SettingsView.h"
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
    // std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSetupComp;
    // juce::AudioDeviceManager audioDeviceManager;
    juce::TextButton button;
    juce::TabbedComponent mainScreens{juce::TabbedButtonBar::TabsAtBottom};
    AmpView ampView;
    SettingsView settingsView;
    FontWebLookAndFeel fontWebLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlackLoungeAudioProcessorEditor)
};
