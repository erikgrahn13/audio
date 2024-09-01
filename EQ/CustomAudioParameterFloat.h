#pragma once

#include <JuceHeader.h>

class CustomAudioParameterFloat : public juce::AudioParameterFloat
{
  public:
    CustomAudioParameterFloat(const juce::String &parameterID, const juce::String &parameterName,
                              NormalisableRange<float> normalisableRange, float defaultValue, int index = -1)
        : juce::AudioParameterFloat(parameterID, parameterName, normalisableRange, defaultValue), index(index)
    {
    }

    int getIndex() const
    {
        return index;
    }

  private:
    int index;
};