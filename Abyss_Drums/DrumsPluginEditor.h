#pragma once

#include "DrumsPluginProcessor.h"
#include <JuceHeader.h>
#include "CustomLookAndFeel/CustomStandaloneFilterWindow.h"

//==============================================================================
class DrumsAudioProcessorEditor final : public juce::AudioProcessorEditor
{
  public:
    explicit DrumsAudioProcessorEditor(DrumsAudioProcessor &);
    ~DrumsAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    
    void openSettingsView(const juce::Array<juce::var> &args,
                        juce::WebBrowserComponent::NativeFunctionCompletion completion);
    void playPreviewSample(const juce::Array<juce::var> &args,
                        juce::WebBrowserComponent::NativeFunctionCompletion completion);
    void loadDrumSample(const juce::Array<juce::var> &args,
                        juce::WebBrowserComponent::NativeFunctionCompletion completion);
    void clearSample(const juce::Array<juce::var> &args,
                        juce::WebBrowserComponent::NativeFunctionCompletion completion);

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DrumsAudioProcessor &processorRef;

    juce::WebBrowserComponent webViewComponent;

    std::optional<juce::WebBrowserComponent::Resource> getResource(const juce::String &url);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumsAudioProcessorEditor)
};
