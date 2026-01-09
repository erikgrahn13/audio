#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class SynthSound : public juce::SynthesiserSound
{
  public:
    bool appliesToNote(int midiNoteNumber) override
    {
        std::ignore = midiNoteNumber;
        return true;
    }
    bool appliesToChannel(int midiChannel) override
    {
        std::ignore = midiChannel;
        return true;
    }
};