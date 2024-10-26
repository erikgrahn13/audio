#pragma once
#include <span>
class IDsp
{
  public:
    virtual void prepare(const double sampleRate, const int numSamples, const int numChannels) = 0;
    virtual void process(std::span<const float> input, std::span<float> output, int channel) = 0;
};