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
            auto audioFile = chooser.getResult();

            auto* reader =  audioFormatManager.createReaderFor(audioFile);
            if(reader != nullptr)
            {
                auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

                audioFormatReaderSource.release();
                fileName.setText(audioFile.getFileName(), juce::NotificationType::dontSendNotification);
            }

            juce::Logger::writeToLog(audioFile.getFileName());
        } );
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;
    juce::TextButton loadFileButton;
    juce::Label fileName;
    juce::Slider midiVolume;
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::AudioTransportSource transportSource;
    juce::AudioFormatManager audioFormatManager;
    TransportState state;
    std::unique_ptr<juce::AudioFormatReaderSource> audioFormatReaderSource;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
