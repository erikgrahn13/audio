#pragma once

#include "../IDsp.h"
#include <memory>

class IFilter : public IDsp
{
  public:
    enum Type
    {
        kHighPass,
        kLowPass,
        kLowShelf,
        kHighShelf,
        kPeak,
        kNumTypes
    };

    virtual ~IFilter() = default;

    virtual void setFrequency(const double frequency) = 0;
    virtual void setGain(const double gaindB) = 0;
    virtual void setQ(const double q) = 0;
    virtual void setSampleRate(const double sampleRate) = 0;
    virtual double filterResponse(double sampleRate, double currentFreq) = 0;

    bool isEnabled{true};

  private:
};

std::unique_ptr<IFilter> createFilter(IFilter::Type filterType, double startFrequency, int filterOrder = 2);
