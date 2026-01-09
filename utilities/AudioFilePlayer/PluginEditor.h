#pragma once

#include "CustomLookAndFeel/DeathMetalLookAndFeel.h"
#include "CustomLookAndFeel/FontAudioLookAndFeel.h"
#include "PluginProcessor.h"

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
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select file...", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.wav");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser &chooser) {
                auto audioFile = chooser.getResult();
                if (audioFile.getFileName().isNotEmpty())
                {
                    if (processorRef.loadFile(audioFile))
                    {
                        juce::Logger::writeToLog(audioFile.getFileName());
                        fileName.setText(audioFile.getFullPathName(), juce::NotificationType::dontSendNotification);
                    }
                    else
                    {
                        juce::NativeMessageBox::showMessageBoxAsync(
                            juce::AlertWindow::WarningIcon, "Error",
                            "The file \"" + audioFile.getFileName() +
                                "\" is encoded in a format that isn't supported.");
                        fileName.setText("No file loaded", juce::NotificationType::dontSendNotification);
                    }
                }
            });
    }

    void playButtonClicked()
    {
    }

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor &processorRef;
    DeathMetalLookAndFeel deathMetalLookAndFeel;
    FontAudioLookAndFeel fontAudioLookAndFeel;
    juce::TextButton loadFileButton;
    juce::TextButton playButton;
    juce::Slider gainSlider;
    juce::TextButton loopButton;
    juce::Label fileName;
    std::unique_ptr<juce::FileChooser> fileChooser;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> gainAttachment;
    std::unique_ptr<ButtonAttachment> loopAttachment;
    std::unique_ptr<ButtonAttachment> playAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
