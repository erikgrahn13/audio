#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class AnalyzerCurve : public juce::Component, juce::Timer {

public:
    AnalyzerCurve(AudioPluginAudioProcessor& processor);
    ~AnalyzerCurve();
    void timerCallback() override;
    void paint(juce::Graphics& g) override;

private:
    void updateBuffer();
    juce::AudioBuffer<float> audioSamples{1, 2048*2};
    AudioPluginAudioProcessor& mProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalyzerCurve)
};
