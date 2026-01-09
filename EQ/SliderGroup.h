#pragma once

#include "CustomLookAndFeel/DeathMetalLookAndFeel.h"
#include "CustomLookAndFeel/FontAudioLookAndFeel.h"
#include "PluginProcessor.h"

class SliderGroup : public juce::Component
{
  public:
    SliderGroup(AudioPluginAudioProcessor &processor, std::string_view bypassParameterID,
                std::string_view frequencyParameterID, std::string_view gainParameterID, std::string_view QParameterID,
                std::string_view filterOrderParameterID);
    ~SliderGroup() override;
    void resized() override;
    void setText(const juce::String &title);
    void setSymbolType(const juce::CharPointer_UTF8 &symbolType);

  private:
    AudioPluginAudioProcessor &mProcessor;
    juce::GroupComponent frame;

    juce::Label filterSymbolLabel;

    // Frequency
    juce::Slider mFrequencySlider;

    // Gain
    juce::Slider mGainSlider;

    // Q
    juce::Slider mQSlider;

    // Bypass
    juce::TextButton mBypassButton;

    // Filter order
    juce::ComboBox mFilterOrder;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mBypassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mFilterOrderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mFrequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mQAttachment;
    DeathMetalLookAndFeel deathMetalLookAndFeel;
    FontAudioLookAndFeel fontAudioLookAndFeel;
};