#pragma once

#include <functional>

class Oscillator
{
public:
    enum OscType
    {
        SINE,
        SQUARE,
        SAW,
        TRIANGLE,
        NUM_TYPES
    };

    Oscillator(OscType oscType);
    float generateSample();
    void setAmplitude(float amplitude) { mAmplitude = amplitude; }
    void setFrequency(float frequency) { mFrequency = frequency; }
    void setType(OscType oscType);
    void setSampleRate(float sampleRate) { mSampleRate = sampleRate; }

private:
    OscType mOscType;
    float mAmplitude;
    float mFrequency;
    float mAngle;
    float mSampleRate;
    std::function<float()> generateSampleFunc;

    // Lambdas for waveform generation
    std::function<float()> sineFunc;
    std::function<float()> squareFunc;
    std::function<float()> sawFunc;
    std::function<float()> triangleFunc;
};
