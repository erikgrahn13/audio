#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

class AnalyzerCurve : public juce::Component, juce::Timer
{
  public:
    AnalyzerCurve(AudioPluginAudioProcessor &processor);
    ~AnalyzerCurve();

    void paint(juce::Graphics &g) override;
    void resized() override;
    void timerCallback() override;

  private:
    AudioPluginAudioProcessor &mProcessor;
    juce::dsp::FFT mFFT{12};
    juce::dsp::WindowingFunction<float> mWindow{static_cast<size_t>(mFFT.getSize()),
                                                juce::dsp::WindowingFunction<float>::hann};
    juce::AudioBuffer<float> mFFTBuffer{1, mFFT.getSize() * 2};
    juce::AudioBuffer<float> avgFFTBuffer{5, mFFT.getSize() / 2};
    int avgFFTBufferPtr = 1;

    juce::Path mFFTPath;
    juce::CriticalSection pathCreationLock;
    double mSampleRate;

    struct FFTPoint
    {
        int firstBinIndex = 0;
        int lastBinIndex = 120;

        int x;
    };

    int fftPointsSize = 0;
    std::vector<FFTPoint> mFFTPoints;

    float getFFTPointLevel(const float *buffer, const FFTPoint &point);

    static constexpr float maxdB = 0.0;
    static constexpr float mindB = -100.0;

    void drawNextFrame();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyzerCurve)
};