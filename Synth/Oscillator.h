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
        TRIANGLE
    };

    Oscillator(OscType oscType);
    float generateSample();
    void setAmplitude(float amplitude) { mAmplitude = amplitude; };
    void setFrequency(float frequency) { mFrequency = frequency; };

private:
    OscType mOscType;
    float mAmplitude;
    float mFrequency;
    float mAngle;
    float mSampleRate;
    std::function<float()> generateSampleFunc;
};