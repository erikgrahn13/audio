#pragma once

#include "DeathMetalLookAndFeel.h"
#include "FontAudioLookAndFeel.h"
#include "PluginProcessor.h"
#include <JuceHeader.h>

class SliderGroup : public juce::Component
{
  public:
    SliderGroup(AudioPluginAudioProcessor &processor, juce::AudioProcessorValueTreeState &parameters,
                std::string_view frequencyParameterID, std::string_view gainParameterID, std::string_view QParameterID);
    ~SliderGroup();
    void resized() override;
    void setText(const juce::String &title);
    void setSymbolType(const juce::CharPointer_UTF8 &symbolType);

  private:
    AudioPluginAudioProcessor &mProcessor;
    juce::AudioProcessorValueTreeState &mParameters;
    juce::GroupComponent frame;

    juce::Label filterSymbolLabel;

    // Frequency
    juce::Slider mFrequencySlider;

    // Gain
    juce::Slider mGainSlider;

    // Q
    juce::Slider mQSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mFrequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mQAttachment;
    DeathMetalLookAndFeel deathMetalLookAndFeel;
    FontAudioLookAndFeel fontAudioLookAndFeel;
};