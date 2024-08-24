#pragma once

#include "../IDsp.h"
#include <array>
#include <vector>

class Biquad : public IDsp
{
  public:
    enum Type
    {
        kLowpass,
        kHighpass,
        kLowShelf,
        kHighShelf,
        kPeak,
        kNumTypes
    };

    Biquad(Type type, double startFreq);
    void prepare(const double sampleRate, const int numSamples, const int numChannels);
    void process(std::span<const float> input, std::span<float> output, int channel) override;
    void reset();
    void setAllParams(double frequency, double gaindB, double q);
    void setFrequency(double frequency);
    void setQ(double q);
    void setGain(double gaindB);
    void setSampleRate(double sampleRate);
    static double filterResponse(double sampleRate, double x, Biquad &biquad);
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
