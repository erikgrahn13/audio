#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);
    getLookAndFeel().setUsingNativeAlertWindows(true);

    loadFileButton.onClick = [this]()
    { loadFileButtonClicked(); };
    loadFileButton.setButtonText("Load File");
    addAndMakeVisible(&loadFileButton);

    playButton.onClick = [this]()
    {
        processorRef.getTransportSource().start();
    };
    playButton.setButtonText("Play");
    addAndMakeVisible(&playButton);

    stopButton.onClick = [this]()
    {
        processorRef.getTransportSource().stop();
        processorRef.getTransportSource().setPosition(0.0);
    };
    stopButton.setButtonText("Stop");
    addAndMakeVisible(&stopButton);

    loopButton.setButtonText("Loop");
    loopButton.onClick = [this]()
    {
        processorRef.getAudioFormatReaderSource().setLooping(loopButton.getToggleState());
    };
    loopButton.setClickingTogglesState(true);
    addAndMakeVisible(loopButton);

    fileName.setText("No file loaded", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(&fileName);

    volume.setSliderStyle(juce::Slider::LinearBarVertical);
    volume.setRange(-40.0, 0.0, 0.01);
    volume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    volume.setPopupDisplayEnabled(true, false, this);
    volume.setTextValueSuffix(" Volume");
    volume.setValue(0.0);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
    setResizable(true, true);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hello Grahn!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    fileName.setBounds(10, 10, getWidth() / 2 - 20, 20);
    volume.setBounds(40, 30, 20, getHeight() - 60);
    loadFileButton.setBounds(getWidth() / 2, 10, getWidth() / 2 - 20, 20);

    playButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
    loopButton.setBounds(10, 100, getWidth() - 20, 20);
}
