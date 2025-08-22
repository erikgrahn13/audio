#pragma once

#include "AbyssPluginProcessor.h"

//==============================================================================
class AbyssAudioProcessorEditor final : public juce::AudioProcessorEditor
{
  public:
    explicit AbyssAudioProcessorEditor(AbyssAudioProcessor &);
    ~AbyssAudioProcessorEditor() override;

    //==============================================================================
    // void paint(juce::Graphics &) override;
    void resized() override;

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AbyssAudioProcessor &processorRef;
    juce::WebSliderRelay gainRelay{"gain"};
    juce::WebToggleButtonRelay bypassRelay{"bypass"};
    juce::WebBrowserComponent webBrowserComponent;
    std::unique_ptr<juce::WebSliderParameterAttachment> gainWebAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> bypassWebAttachment;

    std::optional<juce::WebBrowserComponent::Resource> getResource(const juce::String &url);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AbyssAudioProcessorEditor)
};
