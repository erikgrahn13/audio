#pragma once

#include "../IDsp.h"
#include <tuple>

class NoiseGate : public IDsp
{

  public:
    NoiseGate(double thresholdDB, double sampleRate);
    ~NoiseGate();

    void setThreshold(double value);
    float process(float sample, int channel = 1);
    void prepare(const double sampleRate, const int numSamples, const int numChannels) override
    {
        std::ignore = sampleRate;
        std::ignore = numSamples;
        std::ignore = numChannels;
    }
    void process(std::span<const float> input, std::span<float>, int channel) override
    {
        std::ignore = input;
        std::ignore = channel;
    }

  private:
    double mThresholdLinear;
    double mAlphaAttack;
    double mAlphaRelease;
    double mHold;
    double gs_prev;
    double mCA;
};