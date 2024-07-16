#include "SynthVoice.h"
#include <ranges>

SynthVoice::SynthVoice() : oscillator(Oscillator::OscType::SINE)
{
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound *sound)
{
    return dynamic_cast<SynthSound *>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *, int currentPitchWheelPosition)
{
    juce::ignoreUnused(velocity, currentPitchWheelPosition);
    oscillator.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    oscillator.setAmplitude(0.05f);
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    juce::ignoreUnused(allowTailOff, velocity);
    clearCurrentNote();
    oscillator.setAmplitude(0.0f);
}

void SynthVoice::pitchWheelMoved(int) {}
void SynthVoice::controllerMoved(int, int) {}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    if (!isVoiceActive())
        return;

    for (auto sample : std::ranges::iota_view{0, numSamples})
    {
        auto currentSample = oscillator.generateSample();

        for (auto channel : std::ranges::iota_view{0, outputBuffer.getNumChannels()})
        {
            outputBuffer.addSample(channel, startSample, currentSample);
        }

        ++startSample;
    }
}

void SynthVoice::setCurrentPlaybackSampleRate(const double newRate)
{
    oscillator.setSampleRate(newRate);
}
