#include "Biquad.h"
#include <cmath>
#include <complex>
#include <numbers>
#include <tuple>

Biquad::Biquad(Type type, double startFreq)
    : type(type), mFrequency(startFreq), mGaindB(0.), mQ(0.707), mSampleRate(48000)
{
    setAllParams(mFrequency, mGaindB, mQ);
}

void Biquad::prepare(const double sampleRate, const int numSamples, const int numChannels)
{
    mNumSamples = numSamples;
    std::ignore = numChannels;

    if (mNumChannels != numChannels)
    {
        xn_1.resize(numChannels, 0.0);
        xn_2.resize(numChannels, 0.0);
        yn_1.resize(numChannels, 0.0);
        yn_2.resize(numChannels, 0.0);
    }

    mSampleRate = sampleRate;
}

void Biquad::process(std::span<const float> input, std::span<float> output, int channel)
{
    size_t numSamples = input.size(); // numSamples is derived from the span size
    for (size_t i = 0; i < numSamples; ++i)
    {
        double sample = input[i];
        double filteredSample =
            (mCoeffs.b.at(0) * sample + mCoeffs.b.at(1) * xn_1.at(channel) + mCoeffs.b.at(2) * xn_2.at(channel) -
             mCoeffs.a.at(1) * yn_1.at(channel) - mCoeffs.a.at(2) * yn_2.at(channel)) /
            mCoeffs.a.at(0);

        yn_2.at(channel) = yn_1.at(channel);
        yn_1.at(channel) = filteredSample;
        xn_2.at(channel) = xn_1.at(channel);
        xn_1.at(channel) = sample;

        output[i] = filteredSample;
    }
}

void Biquad::reset()
{
}

void Biquad::setAllParams(double frequency, double gaindB, double q)
{
    if (mFrequency != frequency || mGaindB != gaindB || mQ != q)
    {
        mFrequency = frequency;
        mGaindB = gaindB;
        mQ = q;
        updateCoefficients();
    }
}

void Biquad::setFrequency(double frequency)
{
    if (mFrequency != frequency)
    {
        mFrequency = frequency;
        updateCoefficients();
    }
}

void Biquad::setQ(double q)
{
    if (mQ != q)
    {
        mQ = q;
        updateCoefficients();
    }
}

void Biquad::setGain(double gaindB)
{
    if (mGaindB != gaindB)
    {
        mGaindB = gaindB;
        updateCoefficients();
    }
}

void Biquad::updateCoefficients()
{
    const double w0 = (2 * std::numbers::pi * mFrequency) / mSampleRate;
    const double cosw0 = std::cos(w0);
    const double sinw0 = std::sin(w0);
    double alpha = sinw0 / (2.0 * mQ);
    double A = std::pow(10, mGaindB / 40);

    switch (type)
    {
    case kHighpass:
        mCoeffs.b.at(0) = (1.0 + cosw0) * 0.5;
        mCoeffs.b.at(1) = -(1.0 + cosw0);
        mCoeffs.b.at(2) = (1.0 + cosw0) * 0.5;
        mCoeffs.a.at(0) = 1.0 + alpha;
        mCoeffs.a.at(1) = -2.0 * cosw0;
        mCoeffs.a.at(2) = 1.0 - alpha;
        break;
    case kLowpass:
        mCoeffs.b.at(0) = (1.0 - cosw0) * 0.5;
        mCoeffs.b.at(1) = 1.0 - cosw0;
        mCoeffs.b.at(2) = (1.0 - cosw0) * 0.5;
        mCoeffs.a.at(0) = 1.0 + alpha;
        mCoeffs.a.at(1) = -2.0 * cosw0;
        mCoeffs.a.at(2) = 1.0 - alpha;
        break;
    case kLowShelf:
        mCoeffs.b.at(0) = A * ((A + 1) - (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha);
        mCoeffs.b.at(1) = 2 * A * ((A - 1) - (A + 1) * cosw0);
        mCoeffs.b.at(2) = A * ((A + 1) - (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha);
        mCoeffs.a.at(0) = (A + 1) + (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha;
        mCoeffs.a.at(1) = -2 * ((A - 1) + (A + 1) * cosw0);
        mCoeffs.a.at(2) = (A + 1) + (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha;
        break;
    case kHighShelf:
        mCoeffs.b.at(0) = A * ((A + 1) + (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha);
        mCoeffs.b.at(1) = -2 * A * ((A - 1) + (A + 1) * cosw0);
        mCoeffs.b.at(2) = A * ((A + 1) + (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha);
        mCoeffs.a.at(0) = (A + 1) - (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha;
        mCoeffs.a.at(1) = 2 * ((A - 1) - (A + 1) * cosw0);
        mCoeffs.a.at(2) = (A + 1) - (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha;
        break;
    case kPeak:
        mCoeffs.b.at(0) = 1 + alpha * A;
        mCoeffs.b.at(1) = -2 * cosw0;
        mCoeffs.b.at(2) = 1 - alpha * A;
        mCoeffs.a.at(0) = 1 + alpha / A;
        mCoeffs.a.at(1) = -2 * cosw0;
        mCoeffs.a.at(2) = 1 - alpha / A;
        break;
    default:
        break;
    }
}

void Biquad::setSampleRate(double sampleRate)
{
    this->mSampleRate = sampleRate;
    updateCoefficients();
}

std::vector<double> Biquad::getCoefficients()
{
    return {mCoeffs.a.at(0), mCoeffs.a.at(1), mCoeffs.a.at(2), mCoeffs.b.at(0), mCoeffs.b.at(1), mCoeffs.b.at(2)};
}

double Biquad::filterResponse(double sampleRate, double x, Biquad &biquad)
{
    auto logMin = std::log10(20.0);
    auto logMax = std::log10(20000.0);

    auto value = std::pow(10.0, x * (logMax - logMin) + logMin);

    std::complex<double> j(0, 1);
    std::complex<double> numerator = 0.0;
    std::complex<double> denominator = 0.0;
    std::complex<double> factor = 1.0;

    std::complex<double> jw = std::exp(-2 * std::numbers::pi * value * j / sampleRate);

    numerator = biquad.mCoeffs.b.at(0) + biquad.mCoeffs.b.at(1) * jw + biquad.mCoeffs.b.at(2) * std::pow(jw, 2);
    denominator = biquad.mCoeffs.a.at(0) + biquad.mCoeffs.a.at(1) * jw + biquad.mCoeffs.a.at(2) * std::pow(jw, 2);

    auto magnitude = 20 * std::log10(std::abs(numerator / denominator));

    return magnitude;
}
