#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p),
      keyboardComponent(processorRef.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setSize(800, 600);

    sineButton.setLookAndFeel(&fontAudioLookAndFeel);
    sineButton.setRadioGroupId(1);
    sineButton.setClickingTogglesState(true);
    sineButton.setButtonText(juce::CharPointer_UTF8(""));
    sineButton.setConnectedEdges(Button::ConnectedOnRight);
    addAndMakeVisible(sineButton);

    squareButton.setLookAndFeel(&fontAudioLookAndFeel);
    squareButton.setRadioGroupId(1);
    squareButton.setClickingTogglesState(true);
    squareButton.setButtonText(juce::CharPointer_UTF8(""));
    squareButton.setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    addAndMakeVisible(squareButton);

    sawButton.setLookAndFeel(&fontAudioLookAndFeel);
    sawButton.setRadioGroupId(1);
    sawButton.setClickingTogglesState(true);
    sawButton.setButtonText(juce::CharPointer_UTF8(""));
    sawButton.setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    addAndMakeVisible(sawButton);

    triangleButton.setLookAndFeel(&fontAudioLookAndFeel);
    triangleButton.setRadioGroupId(1);
    triangleButton.setClickingTogglesState(true);
    triangleButton.setButtonText(juce::CharPointer_UTF8(""));
    triangleButton.setConnectedEdges(Button::ConnectedOnLeft);
    addAndMakeVisible(triangleButton);

    // Initially select the Sine waveform
    sineButton.setToggleState(true, juce::NotificationType::dontSendNotification);

    auto oscType = processorRef.mParameters.getParameter("oscType");

    sineButton.onClick = [&]() {
        if (sineButton.getToggleState())
        {
            oscType->beginChangeGesture();
            oscType->setValueNotifyingHost(static_cast<float>(oscType->convertTo0to1(Oscillator::OscType::SINE)));
            oscType->endChangeGesture();
        }
    };

    squareButton.onClick = [&]() {
        if (squareButton.getToggleState())
        {
            oscType->beginChangeGesture();
            oscType->setValueNotifyingHost(static_cast<float>(oscType->convertTo0to1(Oscillator::OscType::SQUARE)));
            oscType->endChangeGesture();
        }
    };

    sawButton.onClick = [&]() {
        if (sawButton.getToggleState())
        {
            oscType->beginChangeGesture();
            oscType->setValueNotifyingHost(static_cast<float>(oscType->convertTo0to1(Oscillator::OscType::SAW)));
            oscType->endChangeGesture();
        }
    };

    triangleButton.onClick = [&]() {
        if (triangleButton.getToggleState())
        {
            oscType->beginChangeGesture();
            oscType->setValueNotifyingHost(static_cast<float>(oscType->convertTo0to1(Oscillator::OscType::TRIANGLE)));
            oscType->endChangeGesture();
        }
    };

    addAndMakeVisible(keyboardComponent);
}

SynthAudioProcessorEditor::~SynthAudioProcessorEditor()
{
}

//==============================================================================
void SynthAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void SynthAudioProcessorEditor::resized()
{
    int buttonWidth = 100;
    int buttonHeight = 30;
    int startX = getWidth() / 2 - buttonWidth * 2; // Center buttons horizontally
    int startY = getHeight() / 4 - buttonHeight / 2;

    sineButton.setBounds(startX, startY, buttonWidth, buttonHeight);
    squareButton.setBounds(startX + buttonWidth, startY, buttonWidth, buttonHeight);
    sawButton.setBounds(startX + 2 * buttonWidth, startY, buttonWidth, buttonHeight);
    triangleButton.setBounds(startX + 3 * buttonWidth, startY, buttonWidth, buttonHeight);

    keyboardComponent.setBounds(getLocalBounds().removeFromBottom(getLocalBounds().getHeight() / 2));
}
