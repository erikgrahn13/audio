#pragma once

#include <JuceHeader.h>

class SettingsView : public juce::Component
{
  public:
    SettingsView();
    ~SettingsView();

    void paint(juce::Graphics &g) override;
    void resized() override;

  private:
    juce::Label deviceTypeLabel;
    juce::Label outputDeviceLabel;
    juce::Label inputDeviceLabel;
    juce::ComboBox deviceTypeDropDown;
    juce::ComboBox outputDeviceDropDown;
    juce::ComboBox inputDeviceDropDown;

    juce::Label outputChannelsLabel;
    juce::ComboBox outputChannelsLeftDropDown;
    juce::ComboBox outputChannelsRightDropDown;
};