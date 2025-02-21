#pragma once

#include "AbyssPluginProcessor.h"

class MyWebBrowserComponent : public juce::WebBrowserComponent
{
  public:
    MyWebBrowserComponent(const Options &options) : juce::WebBrowserComponent(options)
    {
    }
    // bool pageAboutToLoad(const juce::String &newURL) override
    // {
    //     DBG("Loading URL: " << newURL);

    //     // Block reloads of the same page
    //     if (newURL == lastLoadedURL)
    //         return true; // Prevent reload

    //     lastLoadedURL = newURL;
    //     return false; // Allow first load
    // }
    juce::String lastLoadedURL;
};

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
    // juce::WebBrowserComponent webBrowserComponent;
    MyWebBrowserComponent webBrowserComponent;

    std::optional<juce::WebBrowserComponent::Resource> getResource(const juce::String &url);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AbyssAudioProcessorEditor)
};
