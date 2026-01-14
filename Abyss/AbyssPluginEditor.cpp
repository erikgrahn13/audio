#include "AbyssPluginEditor.h"
#include "AbyssPluginProcessor.h"
#include "DemoUtilities.h"
#include <JuceHeader.h>
#include "AbyssBackgroundImage.h"
#include "Tuner.h"

//==============================================================================
AbyssAudioProcessorEditor::AbyssAudioProcessorEditor(AbyssAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    modelButtonPrevious.setButtonText("<");
    modelButtonNext.setButtonText(">");
    modelButtonPrevious.setLookAndFeel(&deathMetalLookAndFeel);
    modelButtonNext.setLookAndFeel(&deathMetalLookAndFeel);

    modelButtonNext.onClick = [this](){
        auto nextChoice = dynamic_cast<juce::AudioParameterChoice*>(processorRef.mParameters.getParameter("loadedModel"));
        if(nextChoice->getIndex() < nextChoice->choices.size() -1)
        {
            *nextChoice = nextChoice->getIndex() + 1;
            loadedModelLabel.setText(nextChoice->getCurrentChoiceName(), juce::NotificationType::dontSendNotification);
        }
    };

    modelButtonPrevious.onClick = [this](){
        auto previousChoice = dynamic_cast<juce::AudioParameterChoice*>(processorRef.mParameters.getParameter("loadedModel"));
        if(previousChoice->getIndex() > 0)
        {
            *previousChoice = previousChoice->getIndex() - 1;
            loadedModelLabel.setText(previousChoice->getCurrentChoiceName(), juce::NotificationType::dontSendNotification);
        }
    };

    auto choiceParam = dynamic_cast<juce::AudioParameterChoice*>(processorRef.mParameters.getParameter("loadedModel"));

    loadedModelLabel.setText(choiceParam->getCurrentChoiceName(), juce::NotificationType::dontSendNotification);
    loadedModelLabel.setJustificationType(juce::Justification::centred);
    loadedModelLabel.setLookAndFeel(&deathMetalLookAndFeel);

    modelFrame.setText("");
    modelFrame.setLookAndFeel(&deathMetalLookAndFeel);

    auto *gainParam = processorRef.mParameters.getParameter("gain");
    auto *bypassParam = processorRef.mParameters.getParameter("bypass");
    std::ignore = gainParam;
    std::ignore = bypassParam;


    if(juce::JUCEApplicationBase::isStandaloneApp())
    {
        settingsButton.setLookAndFeel(&fontWebLookAndFeel);
        settingsButton.setButtonText(juce::CharPointer_UTF8(""));
        settingsButton.onClick = [this]() { juce::StandalonePluginHolder::getInstance()->showAudioSettingsDialog(); };
        addAndMakeVisible(&settingsButton);
    }

    titleLabel.setText("ABYSS AMP", juce::NotificationType::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setLookAndFeel(&deathMetalLookAndFeel);
    // auto font = titleLabel.getFont();
    // font.setHeight(32.0f);
    // titleLabel.setFont(font);
    titleLabel.setFont(deathMetalLookAndFeel.getLabelFont(titleLabel).withHeight(32.0f));

    auto imageInputStream =
        juce::MemoryInputStream(Abyss::background_png, Abyss::background_pngSize, false);
    backgroundImage = juce::ImageFileFormat::loadFrom(imageInputStream);

    frame.setLookAndFeel(&deathMetalLookAndFeel);

    auto denoiserName = processorRef.mParameters.getParameter("denoiser")->getName(16).toUpperCase();
    denoiserLabel.setText(denoiserName, juce::NotificationType::dontSendNotification);
    denoiserLabel.setJustificationType(juce::Justification::centred);
    denoiserLabel.setLookAndFeel(&deathMetalLookAndFeel);
    denoiserLabel.attachToComponent(&denoiserSlider, false);

    mDenoiserActiveButton.setLookAndFeel(&deathMetalLookAndFeel);

    denoiserSlider.setLookAndFeel(&deathMetalLookAndFeel);
    denoiserSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    denoiserSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    denoiserSlider.setSize(60, 60);
    mDenoiserAttachment.reset(new
    juce::AudioProcessorValueTreeState::SliderAttachment(processorRef.mParameters,
                                                                                       "denoiser", denoiserSlider));

    auto gainName = processorRef.mParameters.getParameter("gain")->getName(16).toUpperCase();
    gainLabel.setText(gainName, juce::NotificationType::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setLookAndFeel(&deathMetalLookAndFeel);
    gainLabel.attachToComponent(&gainSlider, false);
    gainSlider.setLookAndFeel(&deathMetalLookAndFeel);
    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gainSlider.setSize(60, 60);
    mGainAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(processorRef.mParameters, "gain",
        gainSlider));


    auto volumeName = processorRef.mParameters.getParameter("volume")->getName(16).toUpperCase();
    volumeLabel.setText(volumeName, juce::NotificationType::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centred);
    volumeLabel.setLookAndFeel(&deathMetalLookAndFeel);
    volumeLabel.attachToComponent(&volumeSlider, false);
    volumeSlider.setLookAndFeel(&deathMetalLookAndFeel);
    volumeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volumeSlider.setSize(60, 60);
    mVolumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(processorRef.mParameters,
                                                                                     "volume", volumeSlider));

    frame.setText("");
    
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(frame);
    addAndMakeVisible(modelFrame);
    addAndMakeVisible(loadedModelLabel);
    addAndMakeVisible(modelButtonPrevious);
    addAndMakeVisible(modelButtonNext);
    addAndMakeVisible(mDenoiserActiveButton);
    addAndMakeVisible(denoiserSlider);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(volumeSlider);

    setResizable(true, false);
    setSize(480, 540);
    setResizeLimits(480, 540, 480, 540); // Lock dimensions


    if (juce::JUCEApplicationBase::isStandaloneApp())
    {
        if (auto *pluginHolder = juce::StandalonePluginHolder::getInstance())
        {
            pluginHolder->getMuteInputValue().setValue(false);
        }
    }
}

AbyssAudioProcessorEditor::~AbyssAudioProcessorEditor()
{
}

void AbyssAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);

    
    if (backgroundImage.isValid())
    {
        g.drawImage(backgroundImage, imageArea);

        // Draw dark overlay
        g.setColour(juce::Colours::black.withAlpha(0.6f));
        g.fillRect(imageArea);
        
        // Add gradient vignette to blend edges with background
        float fadeWidth = 60.0f; // Adjust this to control fade distance
        
        // Left fade
        g.setGradientFill(juce::ColourGradient(juce::Colours::black, imageArea.getX(), imageArea.getCentreY(),
                                               juce::Colours::transparentBlack, imageArea.getX() + fadeWidth, imageArea.getCentreY(), false));
        g.fillRect(juce::Rectangle<float>(imageArea.getX(), imageArea.getY(), fadeWidth, imageArea.getHeight()));
        
        // Right fade
        g.setGradientFill(juce::ColourGradient(juce::Colours::transparentBlack, imageArea.getRight() - fadeWidth, imageArea.getCentreY(),
                                               juce::Colours::black, imageArea.getRight(), imageArea.getCentreY(), false));
        g.fillRect(juce::Rectangle<float>(imageArea.getRight() - fadeWidth, imageArea.getY(), fadeWidth, imageArea.getHeight()));
        
        // Top fade
        g.setGradientFill(juce::ColourGradient(juce::Colours::black, imageArea.getCentreX(), imageArea.getY(),
                                               juce::Colours::transparentBlack, imageArea.getCentreX(), imageArea.getY() + fadeWidth, false));
        g.fillRect(juce::Rectangle<float>(imageArea.getX(), imageArea.getY(), imageArea.getWidth(), fadeWidth));
        
        // Bottom fade
        g.setGradientFill(juce::ColourGradient(juce::Colours::transparentBlack, imageArea.getCentreX(), imageArea.getBottom() - fadeWidth,
                                               juce::Colours::black, imageArea.getCentreX(), imageArea.getBottom(), false));
        g.fillRect(juce::Rectangle<float>(imageArea.getX(), imageArea.getBottom() - fadeWidth, imageArea.getWidth(), fadeWidth));
    }

    g.setFont(deathMetalLookAndFeel.getLabelFont(titleLabel).withHeight(32.0f));
    g.setColour(juce::Colours::white);
    g.drawFittedText("ABYSS AMP", titleLabelBounds.reduced(20), juce::Justification::centredTop, 1);
    // g.fillRect(getLocalBounds());
    // g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);
}


void AbyssAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getBounds();

    titleLabelBounds = bounds.removeFromTop(bounds.getHeight() / 4);

    imageArea = bounds.removeFromTop(bounds.getHeight() * 2 / 3).toFloat();



    auto knobArea = bounds;
    
    frame.setBounds(knobArea);
    
    // Position sliders within the GroupComponent's content area
    // GroupComponent reserves space for text at top and has default indentation
    auto frameContent = knobArea;
    frameContent.removeFromTop(20);  // Space for group text/border
    frameContent = frameContent.reduced(15, 10);  // Horizontal and vertical padding
    
    int sliderSize = 60;
    int buttonSize = 26;
    int toggleSize = 50;
    int denoiserGroupWidth = toggleSize + 5 + sliderSize;  // Toggle + gap + slider
    int numItems = 4;  // Model selector + denoiser group + gain + volume
    int totalItemsWidth = sliderSize + denoiserGroupWidth + sliderSize + sliderSize;
    int spacing = (frameContent.getWidth() - totalItemsWidth) / (numItems + 1);
    
    // Position controls on the same horizontal line
    int yPos = frameContent.getY() + (frameContent.getHeight() - sliderSize) / 2;
    
    // Model selector - first slot
    int modelSelectorX = frameContent.getX() + spacing;
    int modelSelectorWidth = sliderSize;
    
    // Label at top
    int labelHeight = 20;
    loadedModelLabel.setBounds(modelSelectorX, yPos, modelSelectorWidth, labelHeight);
    
    // Buttons below label, centered and side by side
    int buttonsY = yPos + labelHeight + 4;
    int totalButtonsWidth = buttonSize * 2;
    int buttonsX = modelSelectorX + (modelSelectorWidth - totalButtonsWidth) / 2;
    modelButtonPrevious.setBounds(buttonsX, buttonsY, buttonSize, buttonSize);
    modelButtonNext.setBounds(buttonsX + buttonSize, buttonsY, buttonSize, buttonSize);
    
    // Hide the model frame since we're not using it as a visible container
    modelFrame.setVisible(false);
    
    // Denoiser group - toggle to the left of slider
    int denoiserGroupX = modelSelectorX + sliderSize + spacing;
    int toggleY = yPos + (sliderSize - toggleSize) / 2;  // Center toggle vertically
    mDenoiserActiveButton.setBounds(denoiserGroupX, toggleY, toggleSize, toggleSize);
    denoiserSlider.setBounds(denoiserGroupX + toggleSize + 5, yPos, sliderSize, sliderSize);
    
    // Remaining controls
    int gainX = denoiserGroupX + denoiserGroupWidth + spacing;
    gainSlider.setBounds(gainX, yPos, sliderSize, sliderSize);
    volumeSlider.setBounds(gainX + sliderSize + spacing, yPos, sliderSize, sliderSize);



    // settingsButton.setBounds(bounds);

}

void AbyssAudioProcessorEditor::timerCallback()
{
    if (processorRef.mRingBuffer->getNumReady() >= 4096)
    {
        std::array<float, 4096> samples;
        processorRef.mRingBuffer->readFromFifo(samples.data(), 4096);

        float pitchHz = Tuner::detectPitchAutocorrelation(samples.data(), 4096, processorRef.getSampleRate());
        // webBrowserComponent.emitEventIfBrowserIsVisible(juce::Identifier("pitch"), pitchHz);
        std::ignore = pitchHz;
    }
}
