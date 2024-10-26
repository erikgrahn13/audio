#pragma once

// #include "Biquad.h"
#include <IFilter.h>
#include <memory>
#include <optional>
#include <span>
#include <vector>

class Filter
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

    enum Order
    {
        k6dBSlope,
        k12dBSlope,
        k18dBSlope,
        k24dbSlope,
    };

    template <int FilterOrder = 2>
    static std::unique_ptr<Filter> createFilterInstance(Type filterType, double startFrequency)
    {
        switch (FilterOrder)
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;

        default:
            // assert here for invalid filter order
            break;
        }

        return std::unique_ptr<Filter>(new Filter(filterType, startFrequency));
    }

    void prepare(double sampleRate, int numSamples, int numChannels);
    void setFrequency(double frequency);
    void setGain(double gain);
    void setQ(double q);
    void setFilterOrder(Order filterOrder);
    void setSampleRate(double sampleRate);
    double filterResponse(double sampleRate, double x);
    Type getType() const
    {
        return mFilterType;
    }

    void process(std::span<const float> input, std::span<float> output, int channel);

  protected:
  private:
    Filter(Type filterType, double startFrequency);
    Type mFilterType;
    std::vector<std::unique_ptr<IFilter>> mFilters;
};
