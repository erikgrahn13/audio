#pragma once

#include "../IDsp.h"
#include <vector>
#include <array>

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
    void prepare(const double sampleRate, const double numSamples, const int numChannels);
    double process(double sample, int channel) override;
    void reset();
    void setFrequency(double frequency);
    void setQ(double q);
    void setGain(double gaindB);
    void setSampleRate(double sampleRate);
    static double filterResponse(double sampleRate, double x, std::array<double, 3> &a, std::array<double, 3> &b);
    std::vector<double> getCoefficients();
    double getFrequency() { return mFrequency; }
    double getSampleRate() { return mSampleRate; }
    Type getType() const { return type; }

    struct Coeffs
    {
        std::array<double, 3> a{};
        std::array<double, 3> b{};
    };

    void setCoeffs(Biquad::Coeffs coeffs);
    struct Coeffs mCoeffs;

private:
    void updateParams(double frequency, double q = 0.707, double gaindB = 0.0);
    Type type;
    double mSampleRate;
    double mFrequency;
    double mQ;
    double mGaindB;
    double xn_1[2]{0};
    double xn_2[2]{0};
    double yn_1[2]{0};
    double yn_2[2]{0};
};
