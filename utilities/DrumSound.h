#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

class DrumSound : public juce::SynthesiserSound
{
public:
    DrumSound(const juce::String& name,
              juce::AudioBuffer<float>&& audioData,
              int midiNote,
              double sampleRate,
              int minVelocity = 0,
              int maxVelocity = 127)
        : soundName(name),
          data(std::move(audioData)),
          sourceSampleRate(sampleRate),
          midiRootNote(midiNote),
          velocityMin(minVelocity),
          velocityMax(maxVelocity)
    {
    }

    bool appliesToNote(int midiNoteNumber) override
    {
        return midiNoteNumber == midiRootNote;
    }

    bool appliesToChannel(int /*midiChannel*/) override
    {
        return true;
    }
    
    bool appliesToVelocity(float velocity) const
    {
        int vel = static_cast<int>(velocity * 127.0f);
        return vel >= velocityMin && vel <= velocityMax;
    }

    const juce::AudioBuffer<float>& getAudioData() const { return data; }
    double getSampleRate() const { return sourceSampleRate; }
    int getNumChannels() const { return data.getNumChannels(); }
    const juce::String& getName() const { return soundName; }

private:
    juce::String soundName;
    juce::AudioBuffer<float> data;  // All 16 channels stored here
    double sourceSampleRate;
    int midiRootNote;
    int velocityMin;
    int velocityMax;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumSound)
};
