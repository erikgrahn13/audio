#pragma once

#include "IFilter.h"
#include <array>
#include <vector>

class Biquad : public IFilter
{
  public:
    static Biquad createHighPass(double startFrequency, int order)
    {
        return Biquad(kHighPass, startFrequency, order);
    }

    static Biquad createLowPass()
    {
    }

    Biquad(IFilter::Type type, double startFreq, int order = 2);

    void prepare(const double sampleRate, const int numSamples, const int numChannels) override;
    void process(std::span<const float> input, std::span<float> output, int channel) override;
    void reset();
    void setAllParams(double frequency, double gaindB, double q);
    void setFrequency(double frequency) override;
    void setQ(double q) override;
    void setGain(double gaindB) override;
    void setSampleRate(double sampleRate) override;
    double filterResponse(double sampleRate, double currentFrequency) override;

    std::vector<double> getCoefficients();
    double getFrequency()
    {
        return mFrequency;
    }

    double getGain()
    {
        return mGaindB;
    }

    double getQ()
    {
        return mQ;
    }

    double getSampleRate()
    {
        return mSampleRate;
    }
    Type getType() const
    {
        return type;
    }

    struct Coeffs
    {
        std::array<double, 3> a{};
        std::array<double, 3> b{};
    };

    struct Coeffs mCoeffs;

  private:
    void updateCoefficients();
    Type type;
    int mFilterOrder;
    double mSampleRate;
    double mFrequency;
    double mQ;
    double mGaindB;
    int mNumChannels;
    int mNumSamples;

    std::vector<double> xn_1;
    std::vector<double> xn_2;
    std::vector<double> yn_1;
    std::vector<double> yn_2;
};
