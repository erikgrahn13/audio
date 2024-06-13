#pragma once

#include <JuceHeader.h>
#include <Biquad.h>
#include <array>
#include "ResponseCurve.h"

class EQView : public juce::Component, juce::Timer
{
    class Handle : public juce::Component
    {
        public:
        static constexpr float handSize = 20.;
        Handle(Biquad::Type type, juce::RangedAudioParameter* freqParam, juce::RangedAudioParameter* gainParam = nullptr, juce::RangedAudioParameter* qParam = nullptr);

        void paint(juce::Graphics& g) override;
        void mouseDown(const juce::MouseEvent& event) override;
        void mouseDrag(const juce::MouseEvent& event) override;
        void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;
        void updateFrequencyPositionFromParameter(float newValue);
        void updateGainPositionFromParameter(float newValue);
        void updateQPositionFromParameter(float newValue);

        juce::RangedAudioParameter* mFreqParameter;
        juce::RangedAudioParameter* mGainParameter;
        juce::RangedAudioParameter* mQParameter;

        Biquad biquad;
        private:
        juce::ComponentDragger dragger;
        juce::ComponentBoundsConstrainer constrainer;

        std::unique_ptr<juce::ParameterAttachment> mFreqAttachment{nullptr};
        std::unique_ptr<juce::ParameterAttachment> mGainAttachment{nullptr};
        std::unique_ptr<juce::ParameterAttachment> mQAttachment{nullptr};
        float mFrequency;
        float mGain;
        float mQ;

        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Handle);
    };

    public:
    EQView(AudioPluginAudioProcessor& processor, juce::AudioProcessorValueTreeState& parameters);
    ~EQView();
    void resized() override;
    void paint(juce::Graphics& g) override;

    void drawGrid(juce::Graphics &g);
    void drawVerticalLines(juce::Graphics& g);
    void drawHorizontalLines(juce::Graphics& g);
    void drawPlotCurve();
    void updateFrequencyResponse();
    std::vector<std::unique_ptr<Handle>>& getHandles() { return mHandles; }

    void timerCallback() override;

    private:
    AudioPluginAudioProcessor& mProcessor;
    juce::AudioProcessorValueTreeState& mParameters;
    juce::Path frequencyResponse;

    std::atomic<bool> parameterChanged{false};

    std::vector<std::unique_ptr<Handle>> mHandles;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQView);
};
