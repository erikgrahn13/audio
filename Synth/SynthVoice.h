#pragma once

#include "Oscillator.h"
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
  public:
    SynthVoice();
    bool canPlaySound(juce::SynthesiserSound *) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound,
                   int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    void setOscillatorType(Oscillator::OscType oscType)
    {
        oscillator.setType(oscType);
    };

    // Bring base class method into scope to avoid hiding it
    using juce::SynthesiserVoice::renderNextBlock;

  private:
    Oscillator oscillator;
};