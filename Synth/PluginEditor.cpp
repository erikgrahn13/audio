#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p), keyboardComponent(processorRef.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setSize(800, 600);

    // Initialize waveform buttons
    sineButton.setRadioGroupId(1); // Set the same group ID for mutual exclusivity
    sineButton.setButtonText("Sine");
    addAndMakeVisible(sineButton);

    squareButton.setRadioGroupId(1);
    squareButton.setButtonText("Square");
    addAndMakeVisible(squareButton);

    sawButton.setRadioGroupId(1);
    sawButton.setButtonText("Saw");
    addAndMakeVisible(sawButton);

    triangleButton.setRadioGroupId(1);
    triangleButton.setButtonText("Triangle");
    addAndMakeVisible(triangleButton);

    // Add listeners for button clicks
    // sineButton.onClick = [this]
    // { waveformSelected(Oscillator::OscType::SINE); };
    // squareButton.onClick = [this]
    // { waveformSelected(Oscillator::OscType::SQUARE); };
    // sawButton.onClick = [this]
    // { waveformSelected(Oscillator::OscType::SAW); };
    // triangleButton.onClick = [this]
    // { waveformSelected(Oscillator::OscType::TRIANGLE); };

    // Initially select the Sine waveform
    sineButton.setToggleState(true, juce::NotificationType::dontSendNotification);

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
