#include "Oscillator.h"
#include <numbers>
#include <cmath>
#include <JuceHeader.h>

Oscillator::Oscillator(OscType oscType) : mOscType(oscType),
                                          mAmplitude(0.0f),
                                          mFrequency(0.0f),
                                          mSampleRate(44100.0f),
                                          mAngle(0.0f)
{
    switch (mOscType)
    {
    case Oscillator::OscType::SINE:
        generateSampleFunc = [this]()
        {
            auto sample = std::sin(mAngle) * mAmplitude;
            mAngle += 2.0 * std::numbers::pi * mFrequency / mSampleRate;
            if (mAngle >= 2.0 * std::numbers::pi)
                mAngle -= 2.0 * std::numbers::pi;
            return sample;
        };
        break;
    case OscType::SQUARE:
        generateSampleFunc = [this]()
        {
            auto sample = (mAngle < std::numbers::pi) ? mAmplitude : -mAmplitude;
            mAngle += 2.0 * std::numbers::pi * mFrequency / mSampleRate;
            if (mAngle >= 2.0 * std::numbers::pi)
                mAngle -= 2.0 * std::numbers::pi;
            return sample;
        };

    case OscType::SAW:
        generateSampleFunc = [this]()
        {
            auto sample = (2.0 * (mAngle / (2.0f * std::numbers::pi)) - 1.0) * mAmplitude;
            mAngle += 2.0 * std::numbers::pi * mFrequency / mSampleRate;
            if (mAngle >= 2.0 * std::numbers::pi)
                mAngle -= 2.0 * std::numbers::pi;
            return sample;
        };
        break;
    case OscType::TRIANGLE:
        generateSampleFunc = [this]()
        {
            auto sample = (2.0 * std::abs(2.0 * (mAngle / (2.0 * std::numbers::pi)) - 1.0f) - 1.0) * mAmplitude;
            mAngle += 2.0 * std::numbers::pi * mFrequency / mSampleRate;
            if (mAngle >= 2.0 * std::numbers::pi)
                mAngle -= 2.0 * std::numbers::pi;
            return sample;
        };
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