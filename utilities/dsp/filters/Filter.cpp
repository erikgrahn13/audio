#include "Filter.h"

Filter::Filter(Type filterType, double startFrequency) : mFilterType(filterType)
{
    auto type = static_cast<IFilter::Type>(filterType);
    switch (filterType)
    {
    case kHighPass:
        mFilters.emplace_back(createFilter(type, startFrequency, 2));
        mFilters.emplace_back(createFilter(type, startFrequency, 2));
        mFilters.back().get()->isEnabled = false; // enable one second order filter (12dB/oct is default)
        mFilters.emplace_back(createFilter(type, startFrequency, 1));
        mFilters.back().get()->isEnabled = false; // enable one second order filter (12dB/oct is default)

        break;
    case kLowPass:
        mFilters.emplace_back(createFilter(type, startFrequency, 2));
        mFilters.emplace_back(createFilter(type, startFrequency, 2));
        mFilters.back().get()->isEnabled = false; // enable one second order filter (12dB/oct is default)
        mFilters.emplace_back(createFilter(type, startFrequency, 1));
        mFilters.back().get()->isEnabled = false; // enable one second order filter (12dB/oct is default)
        break;
    case kLowShelf:
        mFilters.emplace_back(createFilter(type, startFrequency));
        break;
    case kHighShelf:
        mFilters.emplace_back(createFilter(type, startFrequency));
        break;
    case kPeak:
        mFilters.emplace_back(createFilter(type, startFrequency));
        break;
    default:
        break;
    }
}

void Filter::prepare(double sampleRate, int numSamples, int numChannels)
{
    for (auto &filter : mFilters)
    {
        filter->prepare(sampleRate, numChannels, numChannels);
    }
}

void Filter::setFrequency(double frequency)
{
    for (auto &filter : mFilters)
    {
        filter->setFrequency(frequency);
    }
}

void Filter::setGain(double gain)
{
    for (auto &filter : mFilters)
    {
        filter->setGain(gain);
    }
}

void Filter::setQ(double q)
{
    for (auto &filter : mFilters)
    {
        filter->setQ(q);
    }
}

void Filter::setFilterOrder(Order filterOrder)
{
    for (auto &filter : mFilters)
    {
    }

    switch (filterOrder)
    {

    case k6dBSlope:
        mFilters.at(0).get()->isEnabled = false;
        mFilters.at(1).get()->isEnabled = false;
        mFilters.at(2).get()->isEnabled = true;
        break;
    case k12dBSlope:
        mFilters.at(0).get()->isEnabled = true;
        mFilters.at(1).get()->isEnabled = false;
        mFilters.at(2).get()->isEnabled = false;
        break;
    case k18dBSlope:
        mFilters.at(0).get()->isEnabled = true;
        mFilters.at(1).get()->isEnabled = false;
        mFilters.at(2).get()->isEnabled = true;
        break;
    case k24dbSlope:
        mFilters.at(0).get()->isEnabled = true;
        mFilters.at(1).get()->isEnabled = true;
        mFilters.at(2).get()->isEnabled = false;
        break;

    default:
        break;
    }
}

void Filter::setSampleRate(double sampleRate)
{
    for (auto &filter : mFilters)
    {
        filter->setSampleRate(sampleRate);
    }
}

double Filter::filterResponse(double sampleRate, double x)
{
    double magnitude = 0.0;
    for (auto &filter : mFilters)
    {
        if (filter->isEnabled)
        {
            magnitude += filter->filterResponse(sampleRate, x);
        }
    }

    return magnitude;
}

void Filter::process(std::span<const float> input, std::span<float> output, int channel)
{
    std::span<const float> currentInput = input;
    std::span<float> currentOuput = output;

    for (auto &filter : mFilters)
    {
        if (filter->isEnabled)
        {
            filter->process(currentInput, currentOuput, channel);
            currentInput = currentOuput;
        }
    }
}
