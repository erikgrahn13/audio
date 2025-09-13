#include <JuceHeader.h>

class DrumSampler : public juce::Synthesiser
{
    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override
    {
        const juce::ScopedLock sl(lock);

        for (auto *sound : sounds)
        {
            if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
            {
                // If hitting a note that's still ringing, stop it first (it could be
                // still playing because of the sustain or sostenuto pedal).
                for (auto *voice : voices)
                {
                    if ((voice->getCurrentlyPlayingNote() == midiNoteNumber) && voice->isPlayingChannel(midiChannel) &&
                        (voice->getCurrentlyPlayingSound().get() == sound))
                    {
                        stopVoice(voice, 1.0f, true);
                    }
                }

                startVoice(findFreeVoice(sound, midiChannel, midiNoteNumber, isNoteStealingEnabled()), sound,
                           midiChannel, midiNoteNumber, velocity);
            }
        }
    }
};