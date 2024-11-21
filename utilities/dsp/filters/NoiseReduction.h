#pragma once

#include <juce_dsp/juce_dsp.h>

class NoiseReduction
{
  public:
    NoiseReduction();
    ~NoiseReduction();
    void prepare(const juce::dsp::ProcessSpec &spec);
    void process(juce::AudioBuffer<float> &buffer);

    juce::dsp::LinkwitzRileyFilter<float> mLowBandLowPass;
    juce::dsp::LinkwitzRileyFilter<float> mLowBandAllPass1;
    juce::dsp::LinkwitzRileyFilter<float> mLowBandAllPass2;

    juce::dsp::LinkwitzRileyFilter<float> mMid1BandLowPass;
    juce::dsp::LinkwitzRileyFilter<float> mMid1BandHighPass;
    juce::dsp::LinkwitzRileyFilter<float> mMid1BandAllPass;

    juce::dsp::LinkwitzRileyFilter<float> mMid2BandLowPass;
    juce::dsp::LinkwitzRileyFilter<float> mMid2BandHighPass;
    juce::dsp::LinkwitzRileyFilter<float> mMid2BandAllPass;

    juce::dsp::LinkwitzRileyFilter<float> mHighBandHighPass;
    juce::dsp::LinkwitzRileyFilter<float> mHighBandAllPass1;
    juce::dsp::LinkwitzRileyFilter<float> mHighBandAllPass2;

    std::array<juce::AudioBuffer<float>, 4> filterBuffers;
    juce::dsp::NoiseGate<float> mNoiseGate;

  private:
};