#include "Biquad.h"
#include <numbers>
#include <cmath>
#include <complex>
#include <tuple>

Biquad::Biquad(Type type, double startFreq)
    : type(type), mFrequency(startFreq), mGaindB(0.), mQ(0.707), mSampleRate(48000)
{
    updateParams(mFrequency, mQ, mGaindB);
}

void Biquad::prepare(const double sampleRate, const double numSamples, const int numChannels)
{
    std::ignore = numSamples;
    std::ignore = numChannels;
    mSampleRate = sampleRate;
}

double Biquad::process(double sample, int channel)
{
    double output = (mCoeffs.b[0] * sample + mCoeffs.b[1] * xn_1[channel] + mCoeffs.b[2] * xn_2[channel] - mCoeffs.a[1] * yn_1[channel] - mCoeffs.a[2] * yn_2[channel]) / mCoeffs.a[0];
    yn_2[channel] = yn_1[channel];
    yn_1[channel] = output;
    xn_2[channel] = xn_1[channel];
    xn_1[channel] = sample;

    return output;
}

void Biquad::reset()
{
}

void Biquad::setFrequency(double frequency)
{
    mFrequency = frequency;
    updateParams(mFrequency, mQ, mGaindB);
}

void Biquad::setQ(double q)
{
    mQ = q;
    updateParams(mFrequency, mQ, mGaindB);
}

void Biquad::setGain(double gaindB)
{
    mGaindB = gaindB;
    updateParams(mFrequency, mQ, mGaindB);
}

void Biquad::updateParams(double frequency, double q, double gaindB)
{
    const double w0 = (2 * std::numbers::pi * frequency) / mSampleRate;
    const double cosw0 = std::cos(w0);
    const double sinw0 = std::sin(w0);
    double alpha = sinw0 / (2.0 * q);
    double A = std::pow(10, gaindB / 40);

    switch (type)
    {
    case kHighpass:
        mCoeffs.b[0] = (1.0 + cosw0) * 0.5;
        mCoeffs.b[1] = -(1.0 + cosw0);
        mCoeffs.b[2] = (1.0 + cosw0) * 0.5;
        mCoeffs.a[0] = 1.0 + alpha;
        mCoeffs.a[1] = -2.0 * cosw0;
        mCoeffs.a[2] = 1.0 - alpha;
        break;
    case kLowpass:
        mCoeffs.b[0] = (1.0 - cosw0) * 0.5;
        mCoeffs.b[1] = 1.0 - cosw0;
        mCoeffs.b[2] = (1.0 - cosw0) * 0.5;
        mCoeffs.a[0] = 1.0 + alpha;
        mCoeffs.a[1] = -2.0 * cosw0;
        mCoeffs.a[2] = 1.0 - alpha;
        break;
    case kLowShelf:
        mCoeffs.b[0] = A * ((A + 1) - (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha);
        mCoeffs.b[1] = 2 * A * ((A - 1) - (A + 1) * cosw0);
        mCoeffs.b[2] = A * ((A + 1) - (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha);
        mCoeffs.a[0] = (A + 1) + (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha;
        mCoeffs.a[1] = -2 * ((A - 1) + (A + 1) * cosw0);
        mCoeffs.a[2] = (A + 1) + (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha;
        break;
    case kHighShelf:
        mCoeffs.b[0] = A * ((A + 1) + (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha);
        mCoeffs.b[1] = -2 * A * ((A - 1) + (A + 1) * cosw0);
        mCoeffs.b[2] = A * ((A + 1) + (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha);
        mCoeffs.a[0] = (A + 1) - (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha;
        mCoeffs.a[1] = 2 * ((A - 1) - (A + 1) * cosw0);
        mCoeffs.a[2] = (A + 1) - (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha;
        break;
    case kPeak:
        mCoeffs.b[0] = 1 + alpha * A;
        mCoeffs.b[1] = -2 * cosw0;
        mCoeffs.b[2] = 1 - alpha * A;
        mCoeffs.a[0] = 1 + alpha / A;
        mCoeffs.a[1] = -2 * cosw0;
        mCoeffs.a[2] = 1 - alpha / A;
        break;
    default:
        break;
    }
}

void Biquad::setSampleRate(double sampleRate)
{
    this->mSampleRate = sampleRate;
}

std::vector<double> Biquad::getCoefficients()
{
    return {mCoeffs.a[0], mCoeffs.a[1], mCoeffs.a[2], mCoeffs.b[0], mCoeffs.b[1], mCoeffs.b[2]};
}

void Biquad::setCoeffs(Biquad::Coeffs coeffs)
{
    mCoeffs = coeffs;
}

double Biquad::filterResponse(double sampleRate, double x, std::array<double, 3> &a, std::array<double, 3> &b)
{
    auto logMin = std::log10(20.0);
    auto logMax = std::log10(20000.0);

    auto value = std::pow(10.0, x * (logMax - logMin) + logMin);

    std::complex<double> j(0, 1);
    std::complex<double> numerator = 0.0;
    std::complex<double> denominator = 0.0;
    std::complex<double> factor = 1.0;

    std::complex<double> jw = std::exp(-2 * std::numbers::pi * value * j / sampleRate);

    numerator = b[0] + b[1] * jw + b[2] * std::pow(jw, 2);
    denominator = a[0] + a[1] * jw + a[2] * std::pow(jw, 2);

    auto magnitude = 20 * std::log10(std::abs(numerator / denominator));

    return magnitude;
}
