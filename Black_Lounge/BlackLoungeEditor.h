#pragma once

#include "BlackLoungeProcessor.h"
#include "CustomLookAndFeel/CustomStandaloneFilterWindow.h"
#include "Tuner.h"
#include <JuceHeader.h>

//==============================================================================
class BlackLoungeAudioProcessorEditor final : public juce::AudioProcessorEditor, juce::Timer
{
  public:
    explicit BlackLoungeAudioProcessorEditor(BlackLoungeAudioProcessor &);
    ~BlackLoungeAudioProcessorEditor() override;

    //==============================================================================
    void timerCallback() override;
    void resized() override;
    void nativeFunction(const juce::Array<juce::var> &args,
                        juce::WebBrowserComponent::NativeFunctionCompletion completion);

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BlackLoungeAudioProcessor &processorRef;
    double currentSampleRate;

    // Webview stuff
    juce::WebSliderRelay volumeRelay{"volume"};
    juce::WebSliderRelay gainRelay{"gain"};
    juce::WebSliderRelay denoiserRelay{"denoiser"};
    juce::WebToggleButtonRelay denoiserActiveRelay{"denoiserActive"};
    juce::WebBrowserComponent webBrowserComponent;
    std::unique_ptr<juce::WebSliderParameterAttachment> volumeWebAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> gainWebAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> denoiserWebAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> denoiserActiveAttachment;

    std::optional<juce::WebBrowserComponent::Resource> getResource(const juce::String &url);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlackLoungeAudioProcessorEditor)
};
