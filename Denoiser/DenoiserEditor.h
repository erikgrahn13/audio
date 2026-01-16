#pragma once

#include "DenoiserProcessor.h"
#include "CustomLookAndFeel/DeathMetalLookAndFeel.h"
//==============================================================================
class DenoiserAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit DenoiserAudioProcessorEditor (DenoiserAudioProcessor&);
    ~DenoiserAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    juce::Label titleLabel;
    juce::Image backgroundImage;
    juce::Slider denoiserSlider;
    DeathMetalLookAndFeel deathMetalLookAndFeel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDenoiserAttachment;
    DenoiserAudioProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DenoiserAudioProcessorEditor)
};
