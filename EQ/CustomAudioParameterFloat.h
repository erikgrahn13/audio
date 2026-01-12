#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class CustomAudioParameterFloat : public juce::AudioParameterFloat
{
  public:
    CustomAudioParameterFloat(const juce::ParameterID &parameterID, const juce::String &parameterName,
                              juce::NormalisableRange<float> normalisableRange, float defaultValue, int index = -1)
        : juce::AudioParameterFloat(parameterID, parameterName, normalisableRange, defaultValue), mIndex(index)
    {
    }

    int getIndex() const
    {
        return mIndex;
    }

  private:
    int mIndex;
};