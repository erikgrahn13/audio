#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setSize(800, 300);

    // Add and configure frequency knob
    addAndMakeVisible(freqKnob);
    freqKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    freqKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    freqKnob.setTextValueSuffix(" Hz");
    freqKnob.setRange(80.0, 2000.0, 1.0);
    freqKnob.setValue(processorRef.getFrequency());
    freqKnob.addListener(this);

    // Add and configure frequency label
    addAndMakeVisible(freqLabel);
    freqLabel.setText("Frequency", juce::dontSendNotification);
    freqLabel.setJustificationType(juce::Justification::centred);
    freqLabel.attachToComponent(&freqKnob, false);

    // Add and configure gain knob
    addAndMakeVisible(gainKnob);
    gainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainKnob.setRange(0.0, 0.5, 0.001);
    gainKnob.setValue(processorRef.getAmplitude());
    gainKnob.addListener(this);

    // Add and configure gain label
    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.attachToComponent(&gainKnob, false);
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
    freqKnob.setBounds(10, 40, getWidth() / 2 - 20, getHeight() - 50);
    freqLabel.setBounds(10, 10, getWidth() / 2 - 20, 20);
    gainKnob.setBounds(getWidth() / 2 + 10, 40, getWidth() / 2 - 20, getHeight() - 50);
    gainLabel.setBounds(getWidth() / 2 + 10, 10, getWidth() / 2 - 20, 20);
}



void SynthAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &freqKnob)
        processorRef.setFrequency(freqKnob.getValue());
    else if (slider == &gainKnob)
        processorRef.setAmplitude(gainKnob.getValue());
}
