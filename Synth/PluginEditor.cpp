#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p), keyboardComponent(processorRef.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setSize(800, 600);

    sineButton.setRadioGroupId(1);
    sineButton.setClickingTogglesState(true);
    sineButton.setButtonText("Sine");
    addAndMakeVisible(sineButton);

    squareButton.setRadioGroupId(1);
    squareButton.setClickingTogglesState(true);
    squareButton.setButtonText("Square");
    addAndMakeVisible(squareButton);

    sawButton.setRadioGroupId(1);
    sawButton.setClickingTogglesState(true);
    sawButton.setButtonText("Saw");
    addAndMakeVisible(sawButton);

    triangleButton.setRadioGroupId(1);
    triangleButton.setClickingTogglesState(true);
    triangleButton.setButtonText("Triangle");
    addAndMakeVisible(triangleButton);

    // Initially select the Sine waveform
    // sineButton.setToggleState(true, juce::NotificationType::dontSendNotification);

    sineButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.parameters, "oscType", sineButton);
    squareButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.parameters, "oscType", squareButton);
    sawButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.parameters, "oscType", sawButton);
    triangleButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.parameters, "oscType", triangleButton);

    sineButton.onClick = [this]
    {
        // Log or debug statement to check if the click event is triggered
        std::cout << "Sine button clicked" << std::endl;
    };

    squareButton.onClick = [this]
    {
        // Log or debug statement to check if the click event is triggered
        std::cout << "Square button clicked" << std::endl;
    };

    sawButton.onClick = [this]
    {
        // Log or debug statement to check if the click event is triggered
        std::cout << "Saw button clicked" << std::endl;
    };

    triangleButton.onClick = [this]
    {
        // Log or debug statement to check if the click event is triggered
        std::cout << "Triangle button clicked" << std::endl;
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
