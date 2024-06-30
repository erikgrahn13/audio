#include "Oscillator.h"
#include <numbers>
#include <cmath>

Oscillator::Oscillator(OscType oscType, float amplitude, float freq, float sampleRate) : mOscType(oscType),
                                                                                         mAmplitude(amplitude),
                                                                                         mFrequency(freq),
                                                                                         mSampleRate(sampleRate),
                                                                                         mAngle(0.0f)
{
}

float Oscillator::generateSample()
{
    auto sample = std::sin(mAngle) * mAmplitude;
    mAngle += 2.0 * std::numbers::pi * mFrequency / mSampleRate;
    if (mAngle >= 2.0 * std::numbers::pi)
        mAngle -= 2.0 * std::numbers::pi;
    return sample;
}