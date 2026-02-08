#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include "DrumSound.h"

class DrumSampler : public juce::Synthesiser
{
public:
    // Public method to trigger a note programmatically
    void triggerNote(int midiNoteNumber, float velocity)
    {
        noteOn(1, midiNoteNumber, velocity);
    }

private:
    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override
    {
        const juce::ScopedLock sl(lock);

        // Collect all sounds matching note, channel, AND velocity
        std::vector<juce::SynthesiserSound*> validSounds;
        
        for (auto *sound : sounds)
        {
            if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
            {
                // Check velocity range if it's a DrumSound
                if (auto* drumSound = dynamic_cast<DrumSound*>(sound))
                {
                    if (!drumSound->appliesToVelocity(velocity))
                        continue;
                }
                
                validSounds.push_back(sound);
            }
        }
        
        if (validSounds.empty())
            return;
            
        // Pick a random sound from valid ones
        auto* selectedSound = validSounds[static_cast<size_t>(juce::Random::getSystemRandom().nextInt(static_cast<int>(validSounds.size())))];
        
        // Stop any currently playing instance of the same note
        for (auto *voice : voices)
        {
            if ((voice->getCurrentlyPlayingNote() == midiNoteNumber) && voice->isPlayingChannel(midiChannel) &&
                (voice->getCurrentlyPlayingSound().get() == selectedSound))
            {
                stopVoice(voice, 1.0f, true);
            }
        }

        startVoice(findFreeVoice(selectedSound, midiChannel, midiNoteNumber, isNoteStealingEnabled()), selectedSound,
                   midiChannel, midiNoteNumber, velocity);
    }
};