#pragma once

#include "PluginProcessor.h"
// #include <juce_audio_devices/juce_AudioTransportSource.h>
#include <JuceHeader.h>

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    void loadFileButtonClicked()
    {
        juce::Logger::writeToLog("Button clicked");
        fileChooser = std::make_unique<juce::FileChooser>("Select file...", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.wav");

        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles, [this](const juce::FileChooser &chooser)
                                 {
                                     auto audioFile = chooser.getResult();
                                     if (processorRef.loadFile(audioFile))
                                     {
                                         juce::Logger::writeToLog(audioFile.getFileName());
                                         fileName.setText(audioFile.getFileName(), juce::NotificationType::dontSendNotification);
                                         processorRef.getTransportSource().stop();
                                         processorRef.getTransportSource().setPosition(0.0);
                                     }
                                     else
                                     {
                                           juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                       "Error",
                                                       "The file \"" + audioFile.getFileName() + "\" is encoded in a format that isn't supported.",
                                                       "OK");
                                        fileName.setText("No file loaded", juce::NotificationType::dontSendNotification);
                                     } });
    }

    void playButtonClicked()
    {
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor &processorRef;
    juce::TextButton loadFileButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton loopButton;
    juce::Label fileName;
    juce::Slider volume;
    std::unique_ptr<juce::FileChooser> fileChooser;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
