#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include "DrumSound.h"

class DrumVoice : public juce::SynthesiserVoice
{
public:
    DrumVoice(std::array<juce::AudioParameterFloat*, 16>& channelVolumes,
              std::array<std::atomic<float>, 16>& channelLevels)
        : volumes(channelVolumes), levels(channelLevels)
    {
    }

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<DrumSound*>(sound) != nullptr;
    }

    void startNote(int /*midiNoteNumber*/, float /*velocity*/,
                   juce::SynthesiserSound* s, int /*currentPitchWheelPosition*/) override
    {
        if (auto* drumSound = dynamic_cast<DrumSound*>(s))
        {
            currentSound = drumSound;
            position = 0.0;
            
            // Handle sample rate conversion if needed
            pitchRatio = drumSound->getSampleRate() / getSampleRate();
        }
    }

    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        if (!allowTailOff)
            clearCurrentNote();
    }

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                        int startSample, int numSamples) override
    {
        if (currentSound == nullptr)
            return;

        const auto& sourceBuffer = currentSound->getAudioData();
        const int sourceChannels = juce::jmin(sourceBuffer.getNumChannels(), 16);
        const int sourceSamples = sourceBuffer.getNumSamples();

        for (int i = 0; i < numSamples; ++i)
        {
            const auto sourcePos = static_cast<int>(position);
            if (sourcePos >= sourceSamples)
            {
                clearCurrentNote();
                break;
            }

            // Render each channel with its individual volume
            for (int ch = 0; ch < sourceChannels; ++ch)
            {
                const float sample = sourceBuffer.getSample(ch, sourcePos);
                const float gain = juce::Decibels::decibelsToGain(volumes[static_cast<size_t>(ch)]->get());
                const float outputSample = sample * gain;
                
                // Add to stereo output (mix all channels to L+R)
                outputBuffer.addSample(0, startSample + i, outputSample);  // Left
                outputBuffer.addSample(1, startSample + i, outputSample);  // Right
                
                // Update level meter with RMS-style smoothing
                const float currentLevel = levels[static_cast<size_t>(ch)].load();
                levels[static_cast<size_t>(ch)].store(currentLevel * 0.95f + std::abs(outputSample) * 0.05f);
            }

            position += pitchRatio;
        }
    }

    using SynthesiserVoice::renderNextBlock;

    void pitchWheelMoved(int /*newPitchWheelValue*/) override {}
    void controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/) override {}

private:
    DrumSound* currentSound = nullptr;
    double position = 0.0;
    double pitchRatio = 1.0;
    
    std::array<juce::AudioParameterFloat*, 16>& volumes;
    std::array<std::atomic<float>, 16>& levels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumVoice)
};
