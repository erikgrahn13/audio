#pragma once

enum class OscType
{
    SINE,
    SQUARE,
    SAW,
    TRIANGLE
};

class Oscillator
{
public:
    Oscillator(OscType oscType, float amplitude, float freq, float sampleRate);
    float generateSample();
    void setAmplitude(float amplitude) { mAmplitude = amplitude; };
    void setFrequency(float frequency) { mFrequency = frequency; };

private:
    OscType mOscType;
    float mAmplitude;
    float mFrequency;
    float mAngle;
    float mSampleRate;
};