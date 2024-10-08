#pragma once

#include <JuceHeader.h>

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