#include "Oscillator.h"
#include <JuceHeader.h>
#include <cmath>
#include <numbers>

Oscillator::Oscillator(OscType oscType)
    : mOscType(oscType), mAmplitude(0.0f), mFrequency(0.0f), mSampleRate(44100.0f), mAngle(0.0f)
{
    sineFunc = [this]() {
        auto sample = std::sin(mAngle) * mAmplitude;
        mAngle += 2.f * static_cast<float>(std::numbers::pi) * mFrequency / mSampleRate;
        if (mAngle >= 2.f * static_cast<float>(std::numbers::pi))
            mAngle -= 2.f * static_cast<float>(std::numbers::pi);
        return sample;
    };

    squareFunc = [this]() {
        auto sample = (mAngle < std::numbers::pi) ? mAmplitude : -mAmplitude;
        mAngle += 2.f * static_cast<float>(std::numbers::pi) * mFrequency / mSampleRate;
        if (mAngle >= 2.f * static_cast<float>(std::numbers::pi))
            mAngle -= 2.f * static_cast<float>(std::numbers::pi);
        return sample;
    };

    sawFunc = [this]() {
        auto sample = (2.f * (mAngle / (2.f * static_cast<float>(std::numbers::pi))) - 1.f) * mAmplitude;
        mAngle += 2.f * static_cast<float>(std::numbers::pi) * mFrequency / mSampleRate;
        if (mAngle >= 2.f * static_cast<float>(std::numbers::pi))
            mAngle -= 2.f * static_cast<float>(std::numbers::pi);
        return sample;
    };

    triangleFunc = [this]() {
        auto sample =
            (2.f * std::fabs(2.f * (mAngle / (2.f * static_cast<float>(std::numbers::pi))) - 1.f) - 1.f) * mAmplitude;
        mAngle += 2.f * static_cast<float>(std::numbers::pi) * mFrequency / mSampleRate;
        if (mAngle >= 2.f * static_cast<float>(std::numbers::pi))
            mAngle -= 2.f * static_cast<float>(std::numbers::pi);
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
