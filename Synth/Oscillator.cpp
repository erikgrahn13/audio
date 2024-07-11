#include "Oscillator.h"
#include <cmath>
#include <numbers>
#include <JuceHeader.h>

Oscillator::Oscillator(OscType oscType)
    : mOscType(oscType),
      mAmplitude(0.0f),
      mFrequency(0.0f),
      mSampleRate(44100.0f),
      mAngle(0.0f)
{
    sineFunc = [this]()
    {
        auto sample = std::sin(mAngle) * mAmplitude;
        mAngle += 2.0 * std::numbers::pi * mFrequency / mSampleRate;
        if (mAngle >= 2.0 * std::numbers::pi)
            mAngle -= 2.0 * std::numbers::pi;
        return sample;
    };

    squareFunc = [this]()
    {
        auto sample = (mAngle < std::numbers::pi) ? mAmplitude : -mAmplitude;
        mAngle += 2.0 * std::numbers::pi * mFrequency / mSampleRate;
        if (mAngle >= 2.0 * std::numbers::pi)
            mAngle -= 2.0 * std::numbers::pi;
        return sample;
    };

    sawFunc = [this]()
    {
        auto sample = (2.0 * (mAngle / (2.0f * std::numbers::pi)) - 1.0) * mAmplitude;
        mAngle += 2.0 * std::numbers::pi * mFrequency / mSampleRate;
        if (mAngle >= 2.0 * std::numbers::pi)
            mAngle -= 2.0 * std::numbers::pi;
        return sample;
    };

    triangleFunc = [this]()
    {
        auto sample = (2.0 * std::abs(2.0 * (mAngle / (2.0 * std::numbers::pi)) - 1.0f) - 1.0) * mAmplitude;
        mAngle += 2.0 * std::numbers::pi * mFrequency / mSampleRate;
        if (mAngle >= 2.0 * std::numbers::pi)
            mAngle -= 2.0 * std::numbers::pi;
        return sample;
    };

    // Set the initial generateSampleFunc based on the oscillator type
    setType(oscType);
}

void Oscillator::setType(OscType oscType)
{
    mOscType = oscType;

    // Update generateSampleFunc to point to the appropriate lambda function
    switch (oscType)
    {
    case OscType::SINE:
        generateSampleFunc = sineFunc;
        break;
    case OscType::SQUARE:
        generateSampleFunc = squareFunc;
        break;
    case OscType::SAW:
        generateSampleFunc = sawFunc;
        break;
    case OscType::TRIANGLE:
        generateSampleFunc = triangleFunc;
        break;
    default:
        juce::Logger::writeToLog("Error: Unknown oscillator type!");
        break;
    }
}

float Oscillator::generateSample()
{
    return generateSampleFunc();
}
