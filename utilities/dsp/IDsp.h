#pragma once
#include <span>
class IDsp
{
  public:
    virtual void process(std::span<const float> input, std::span<float> output, int channel) = 0;
};