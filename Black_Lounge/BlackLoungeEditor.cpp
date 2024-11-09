#include "BlackLoungeEditor.h"
#include "BlackLoungeProcessor.h"

//==============================================================================
BlackLoungeAudioProcessorEditor::BlackLoungeAudioProcessorEditor(BlackLoungeAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);
    setSize(400, 300);

    if (juce::JUCEApplicationBase::isStandaloneApp())
    {
        button.setLookAndFeel(&fontWebLookAndFeel);
        button.setButtonText(juce::CharPointer_UTF8(""));
        addAndMakeVisible(button);
        button.onClick = [this] { juce::StandalonePluginHolder::getInstance()->showAudioSettingsDialog(); };
        // auto &erik = juce::StandalonePluginHolder::getInstance()->deviceManager;

        // mainScreens.setOrientation(juce::TabbedButtonBar::Orientation::TabsAtBottom);

        mainScreens.addTab("Erik", juce::Colours::black, &ampView, false);
        mainScreens.addTab("Grahn", juce::Colours::black, &settingsView, false);
        // addAndMakeVisible(mainScreens);
    }

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

    auto thresholdName = processorRef.getVTSParameters().getParameter("threshold")->getName(16).toUpperCase();
    thresholdLabel.setText(thresholdName, juce::NotificationType::dontSendNotification);
    thresholdLabel.setJustificationType(juce::Justification::centred);
    thresholdLabel.setLookAndFeel(&fontDeathMetalLookAndFeel);
    thresholdLabel.attachToComponent(&thresholdSlider, false);
    thresholdSlider.setLookAndFeel(&fontDeathMetalLookAndFeel);
    thresholdSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    thresholdSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mThresholdAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(processorRef.getVTSParameters(),
                                                                                        "threshold", thresholdSlider));

    addAndMakeVisible(thresholdSlider);

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
}

BlackLoungeAudioProcessorEditor::~BlackLoungeAudioProcessorEditor()
{
}

//==============================================================================
void BlackLoungeAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // g.setColour(juce::Colours::white);
    // g.setFont(15.0f);
    // g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    g.setColour(Colours::black);
    g.fillRect(getLocalBounds());
    // g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);
}

void BlackLoungeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor.
    auto r = getLocalBounds().reduced(4);

    mainScreens.setBounds(getLocalBounds());

    auto bounds = getLocalBounds();
    gainSlider.setBounds(20, bounds.getCentreY(), 80, 80);
    thresholdSlider.setBounds(bounds.getCentreX() - 40, bounds.getCentreY(), 80, 80);
    volumeSlider.setBounds(bounds.getWidth() - 100, bounds.getCentreY(), 80, 80);

    // auto knobsArea = bounds.removeFromBottom(bounds.getHeight() / 3);

    // volumeSlider.setBounds(bounds.removeFromRight(bounds.getWidth() / 2).reduced(50));
    // thresholdSlider.setBounds(bounds.reduced(50));

    button.setBounds(10, getHeight() - 70, 60, 60);
    // audioSetupComp->setBounds(r.removeFromTop(proportionOfHeight(0.65f)));
}
