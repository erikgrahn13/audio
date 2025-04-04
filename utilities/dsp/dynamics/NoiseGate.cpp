#include "NoiseGate.h"
#include <cmath>

NoiseGate::NoiseGate(double thresholdDB, double sampleRate)
    : mThresholdLinear(std::pow(10.0, thresholdDB / 20.0)),
      mAlphaAttack(std::exp(-std::log(9.0) / (sampleRate * 0.00001))),
      mAlphaRelease(std::exp(-std::log(9.0) / (sampleRate * 0.05))), mHold(0.05 * sampleRate), gs_prev(0.0), mCA(0.0)
{
}

NoiseGate::~NoiseGate()
{
}

void NoiseGate::setThreshold(double thresholdDB)
{
    mThresholdLinear = std::pow(10.0, thresholdDB / 20.0);
}

float NoiseGate::process(float sample, int channel)
{

    double tmp = std::abs(sample);

    double gc = (tmp < mThresholdLinear) ? 0.0 : 1.0;

    if (gc > mHold && gc <= gs_prev)
    {
        gs_prev = mAlphaAttack * gs_prev + (1.0 - mAlphaAttack) * gc;
        mCA = 0;
    }
    else if (gc <= mHold)
    {
        gs_prev = mAlphaRelease * gs_prev + (1.0 - mAlphaRelease) * gc;
        if (mCA > 0)
        {
            mCA--;
        }
    }
    else
    {
        gs_prev = gc;
        mCA = mHold;
    }

    return gs_prev;
}