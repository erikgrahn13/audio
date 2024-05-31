#include "Oscillator.h"
#include <numbers>
#include <cmath>

Oscillator::Oscillator(OscType oscType) : mOscType(oscType),
                                          mAmplitude(0.0f),
                                          mFrequency(0.0f),
                                          mSampleRate(44100.0f),
                                          mAngle(0.0f)
{
}

float Oscillator::generateSineSample()
{
    auto sample = std::sin(mAngle) * mAmplitude;
    mAngle += 2.0 * std::numbers::pi * mFrequency / mSampleRate;
    if (mAngle >= 2.0 * std::numbers::pi)
        mAngle -= 2.0 * std::numbers::pi;
    return sample;
}