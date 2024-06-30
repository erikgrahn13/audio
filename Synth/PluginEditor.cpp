#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setSize(800, 300);

    frequencySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "FREQUENCY", freqSlider);
    addAndMakeVisible(freqSlider);
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    freqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    freqSlider.setTextValueSuffix(" Hz");
    freqSlider.setRange(80.0, 2000.0, 1.0);
    freqSlider.setValue(processorRef.parameters.getRawParameterValue("FREQUENCY")->load());

    addAndMakeVisible(freqLabel);
    freqLabel.setText("Frequency", juce::dontSendNotification);
    freqLabel.setJustificationType(juce::Justification::centred);
    freqLabel.attachToComponent(&freqSlider, false);

    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "GAIN", gainSlider);
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainSlider.setRange(0.0, 0.5, 0.001);
    gainSlider.setValue(processorRef.parameters.getRawParameterValue("GAIN")->load());

    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.attachToComponent(&gainSlider, false);
}



SynthAudioProcessorEditor::~SynthAudioProcessorEditor()
{
}

//==============================================================================
void SynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void SynthAudioProcessorEditor::resized()
{
    freqSlider.setBounds(10, 40, getWidth() / 2 - 20, getHeight() - 50);
    freqLabel.setBounds(10, 10, getWidth() / 2 - 20, 20);
    gainSlider.setBounds(getWidth() / 2 + 10, 40, getWidth() / 2 - 20, getHeight() - 50);
    gainLabel.setBounds(getWidth() / 2 + 10, 10, getWidth() / 2 - 20, 20);
}
