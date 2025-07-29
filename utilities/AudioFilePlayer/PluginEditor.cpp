#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);

    loadFileButton.setLookAndFeel(&deathMetalLookAndFeel);
    loadFileButton.onClick = [this]() { loadFileButtonClicked(); };
    loadFileButton.setButtonText("LOAD FILE");
    addAndMakeVisible(&loadFileButton);

    playButton.setLookAndFeel(&fontAudioLookAndFeel);
    playButton.setClickingTogglesState(true);
    playButton.onClick = [this]() {
        playButton.getToggleState() ? playButton.setButtonText(juce::CharPointer_UTF8(""))
                                    : playButton.setButtonText(juce::CharPointer_UTF8(""));
    };
    playButton.setButtonText(juce::CharPointer_UTF8(""));
    playAttachment.reset(new ButtonAttachment(processorRef.mParameters, "play", playButton));
    addAndMakeVisible(&playButton);

    loopButton.setLookAndFeel(&fontAudioLookAndFeel);
    loopButton.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::red);
    loopButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);

    loopButton.setButtonText(juce::CharPointer_UTF8(""));
    loopButton.setClickingTogglesState(true);
    loopAttachment.reset(new ButtonAttachment(processorRef.mParameters, "loop", loopButton));
    addAndMakeVisible(loopButton);

    fileName.setText("No file loaded", juce::NotificationType::dontSendNotification);
    fileName.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&fileName);

    gainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    gainSlider.setColour(juce::Slider::trackColourId, juce::Colours::white);
    gainSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::darkgrey);
    gainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);
    gainAttachment.reset(new SliderAttachment(processorRef.mParameters, "gain", gainSlider));
    gainSlider.setLookAndFeel(&deathMetalLookAndFeel);
    addAndMakeVisible(gainSlider);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(300, 150);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();

    auto fileBounds = bounds.removeFromTop(bounds.getHeight() / 3);

    fileName.setBounds(fileBounds.removeFromLeft(bounds.getWidth() / 2));
    loadFileButton.setBounds(fileBounds.reduced(40, 10));

    auto transportBounds = bounds.removeFromTop(bounds.getHeight() / 2);
    playButton.setBounds(transportBounds.removeFromLeft(bounds.getWidth() / 2).reduced(10, 10));
    loopButton.setBounds(transportBounds.reduced(10, 10));

    auto gainBounds = bounds;
    gainSlider.setBounds(gainBounds.reduced(60, 0));
}
