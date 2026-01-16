#include "DenoiserProcessor.h"
#include "DenoiserEditor.h"
#include "DenoiserBackgroundImage.h"

//==============================================================================
DenoiserAudioProcessorEditor::DenoiserAudioProcessorEditor (DenoiserAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    auto imageInputStream = juce::MemoryInputStream(Denoiser::background_png, Denoiser::background_pngSize, false);
    backgroundImage = juce::ImageFileFormat::loadFrom(imageInputStream);

    titleLabel.setText("DENOISER", juce::NotificationType::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setLookAndFeel(&deathMetalLookAndFeel);
    // auto font = titleLabel.getFont();
    // font.setHeight(32.0f);
    // titleLabel.setFont(font);
    titleLabel.setFont(deathMetalLookAndFeel.getLabelFont(titleLabel).withHeight(64.0f));

    // addAndMakeVisible(titleLabel);

    mDenoiserAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        p.mParameters, "denoiser", denoiserSlider
    ));
    denoiserSlider.setLookAndFeel(&deathMetalLookAndFeel);
    denoiserSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    denoiserSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    // denoiserSlider.setSize(10, 10);

    addAndMakeVisible(denoiserSlider);

    setSize (400, 300);
}

DenoiserAudioProcessorEditor::~DenoiserAudioProcessorEditor()
{
}

//==============================================================================
void DenoiserAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    if(backgroundImage.isValid())
    {
        // auto imageArea = getLo
        g.drawImage(backgroundImage, getLocalBounds().reduced(20).toFloat());
    }

    g.setFont(deathMetalLookAndFeel.getLabelFont(titleLabel).withHeight(32.0f));
    g.setColour(juce::Colours::white);
    auto titleLabelBounds = getLocalBounds().removeFromTop(getLocalBounds().getHeight() / 4);
    g.drawFittedText("DENOISER", titleLabelBounds, juce::Justification::centred, 1);

    
    // g.setFont(deathMetalLookAndFeel.getLabelFont(titleLabel).withHeight(32.0f));
    // g.setColour(juce::Colours::white);
    // g.drawFittedText("DENOISER", titleLabelBounds.reduced(20), juce::Justification::centredTop, 1);
}

void DenoiserAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();

    auto titleArea = bounds.removeFromTop(bounds.getHeight() / 4);
    titleLabel.setBounds(titleArea);


    auto sliderArea = bounds.removeFromBottom(bounds.getHeight() / 3);
    denoiserSlider.setBounds(sliderArea);
}
