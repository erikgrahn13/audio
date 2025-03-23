#pragma once

#include <juce_dsp/juce_dsp.h>

class NoiseReduction
{
  public:
    NoiseReduction();
    ~NoiseReduction();
    void prepare(const juce::dsp::ProcessSpec &spec);
    void process(juce::AudioBuffer<float> &buffer);

  private:
    juce::dsp::NoiseGate<float> mNoiseGate;
};