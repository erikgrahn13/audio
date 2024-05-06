#pragma once

#include "PluginProcessor.h"
// #include <juce_audio_devices/juce_AudioTransportSource.h>
#include <JuceHeader.h>

enum class TransportState {
    Stopped,
    Starting,
    Playing,
    Stopping
};



//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void loadFileButtonClicked() 
    {
        juce::Logger::writeToLog("Button clicked");
        fileChooser = std::make_unique<juce::FileChooser>("Select file...", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.wav");
        
        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles ,[this](const juce::FileChooser& chooser)
        {
            juce::File audioFile(chooser.getResult());
            juce::Logger::writeToLog(audioFile.getFileName());
        } );
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;
    juce::TextButton loadFileButton;
    juce::Slider midiVolume;
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::AudioTransportSource transportSource;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
