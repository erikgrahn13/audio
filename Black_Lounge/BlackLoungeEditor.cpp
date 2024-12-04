#include "BlackLoungeEditor.h"
#include "BlackLoungeAmp.h"
#include "BlackLoungeProcessor.h"

//==============================================================================
BlackLoungeAudioProcessorEditor::BlackLoungeAudioProcessorEditor(BlackLoungeAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);
    setSize(500, 400);

    if (juce::JUCEApplicationBase::isStandaloneApp())
    {
        settingsButton.setLookAndFeel(&fontWebLookAndFeel);
        settingsButton.setButtonText(juce::CharPointer_UTF8(""));
        // settingsButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::red);
        addAndMakeVisible(settingsButton);
        settingsButton.onClick = [] { juce::StandalonePluginHolder::getInstance()->showAudioSettingsDialog(); };
    }

    analyzeButton.setButtonText("Analyze");
    mAnalyzeAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(processorRef.getVTSParameters(),
                                                                                      "analyze", analyzeButton));
    addAndMakeVisible(analyzeButton);

    titleLabel.setText("BLACK LOUNGE", juce::NotificationType::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);

    auto fontOptions = juce::FontOptions(
        juce::Typeface::createSystemTypefaceFor(CustomFont::ArtDystopia_ttf, CustomFont::ArtDystopia_ttfSize));
    fontOptions = fontOptions.withHeight(60.);
    titleLabel.setFont(juce::Font(fontOptions));
    // titleLabel.setFont(juce::FontOptions(
    //     juce::Typeface::createSystemTypefaceFor(CustomFont::ArtDystopia_ttf, CustomFont::ArtDystopia_ttfSize)));
    // titleLabel.setFont(juce::Font(juce::FontOptions(
    //     juce::Typeface::createSystemTypefaceFor(CustomFont::ArtDystopia_ttf,
    //     CustomFont::ArtDystopia_ttfSize), 30.f, juce::Font::plain)));

    // Apply the Font to your Label

    // titleLabel.setLookAndFeel(&fontDeathMetalLookAndFeel);
    addAndMakeVisible(titleLabel);

    auto volumeName = processorRef.getVTSParameters().getParameter("volume")->getName(16).toUpperCase();
    volumeLabel.setText(volumeName, juce::NotificationType::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centred);
    volumeLabel.setLookAndFeel(&fontDeathMetalLookAndFeel);
    volumeLabel.attachToComponent(&volumeSlider, false);
    volumeSlider.setLookAndFeel(&fontDeathMetalLookAndFeel);
    volumeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mVolumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(processorRef.getVTSParameters(),
                                                                                     "volume", volumeSlider));
    addAndMakeVisible(volumeSlider);

    auto denoiserName = processorRef.getVTSParameters().getParameter("denoiser")->getName(16).toUpperCase();
    denoiserLabel.setText(denoiserName, juce::NotificationType::dontSendNotification);
    denoiserLabel.setJustificationType(juce::Justification::centred);
    denoiserLabel.setLookAndFeel(&fontDeathMetalLookAndFeel);
    denoiserLabel.attachToComponent(&denoiserSlider, false);
    denoiserSlider.setLookAndFeel(&fontDeathMetalLookAndFeel);
    denoiserSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    denoiserSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mDenoiserAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(processorRef.getVTSParameters(),
                                                                                       "denoiser", denoiserSlider));

    addAndMakeVisible(denoiserSlider);

    auto gainName = processorRef.getVTSParameters().getParameter("gain")->getName(16).toUpperCase();
    gainLabel.setText(gainName, juce::NotificationType::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setLookAndFeel(&fontDeathMetalLookAndFeel);
    gainLabel.attachToComponent(&gainSlider, false);
    gainSlider.setLookAndFeel(&fontDeathMetalLookAndFeel);
    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mGainAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(processorRef.getVTSParameters(), "gain", gainSlider));

    addAndMakeVisible(gainSlider);

    mDenoiserActiveAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
        processorRef.getVTSParameters(), "denoiserActive", mDenoiserActiveButton));

    mDenoiserActiveButton.setLookAndFeel(&fontWebLookAndFeel);
    mDenoiserActiveButton.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::white);
    mDenoiserActiveButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    mDenoiserActiveButton.setButtonText(juce::CharPointer_UTF8(""));
    mDenoiserActiveButton.onClick = [this]() {
        if (mDenoiserActiveButton.getToggleState())
        {
            mDenoiserActiveButton.setButtonText(juce::CharPointer_UTF8(""));
        }
        else
        {
            mDenoiserActiveButton.setButtonText(juce::CharPointer_UTF8(""));
        }
    };
    mDenoiserActiveButton.setClickingTogglesState(true);
    mDenoiserActiveButton.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    mDenoiserActiveButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::transparentBlack);
    mDenoiserActiveButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(mDenoiserActiveButton);

    auto imageInputStream =
        juce::MemoryInputStream(BlackLoungeAmp::background1_png, BlackLoungeAmp::background1_pngSize, false);
    backgroundImage = juce::ImageFileFormat::loadFrom(imageInputStream);
}

BlackLoungeAudioProcessorEditor::~BlackLoungeAudioProcessorEditor()
{
    denoiserLabel.setLookAndFeel(nullptr);
    gainLabel.setLookAndFeel(nullptr);
    volumeLabel.setLookAndFeel(nullptr);
    gainSlider.setLookAndFeel(nullptr);
    volumeSlider.setLookAndFeel(nullptr);
    denoiserSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void BlackLoungeAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // g.setColour(juce::Colours::white);
    // g.setFont(15.0f);
    // g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    // g.setColour(juce::Colours::black);
    auto imagebounds = getLocalBounds().toFloat();
    auto bounds = getLocalBounds().toFloat();

    if (backgroundImage.isValid())
    {
        // g.setOpacity(0.0f);
        imagebounds.removeFromTop(imagebounds.getHeight() / 1 / 6);

        g.drawImage(backgroundImage, imagebounds.removeFromTop(imagebounds.getHeight() * 2 / 3));
        g.setColour(juce::Colours::black.withAlpha(0.25f)); // Adjust the alpha for darkness
        g.fillRect(getLocalBounds());
    }
    // g.fillRect(getLocalBounds());
    // g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);
}

void BlackLoungeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor.

    auto bounds = getLocalBounds();

    titleLabel.setBounds(0, 10, bounds.getWidth(), 100);

    denoiserSlider.setBounds(40, bounds.getCentreY(), 80, 80);
    mDenoiserActiveButton.setBounds(denoiserSlider.getWidth() / 2 + 20,
                                    bounds.getCentreY() + denoiserSlider.getHeight(), 40, 40);

    gainSlider.setBounds(bounds.getCentreX() - 40, bounds.getCentreY() + 40, 80, 80);
    volumeSlider.setBounds(bounds.getWidth() - 120, bounds.getCentreY() + 40, 80, 80);

    // auto knobsArea = bounds.removeFromBottom(bounds.getHeight() / 3);

    // volumeSlider.setBounds(bounds.removeFromRight(bounds.getWidth() / 2).reduced(50));
    // thresholdSlider.setBounds(bounds.reduced(50));

    settingsButton.setBounds(bounds.getWidth() - 50, getHeight() - 50, 40, 40);
}
