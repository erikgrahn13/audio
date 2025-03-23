#include "BlackLoungeEditor.h"
#include "BlackLoungeProcessor.h"
#include "Fonts.h"

//==============================================================================
BlackLoungeAudioProcessorEditor::BlackLoungeAudioProcessorEditor(BlackLoungeAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);
    setSize(400, 300);

    if (juce::JUCEApplicationBase::isStandaloneApp())
    {
        button.setButtonText("Settings");
        addAndMakeVisible(button);
        button.onClick = [this] { juce::StandalonePluginHolder::getInstance()->showAudioSettingsDialog(); };
        // auto &erik = juce::StandalonePluginHolder::getInstance()->deviceManager;

        // mainScreens.setOrientation(juce::TabbedButtonBar::Orientation::TabsAtBottom);
        mainScreens.addTab("Erik", juce::Colours::black, &ampView, false);
        mainScreens.addTab("Grahn", juce::Colours::black, &settingsView, false);
        // addAndMakeVisible(mainScreens);
    }
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
    button.setBounds(10, getHeight() - 30, 100, 20);

    // audioSetupComp->setBounds(r.removeFromTop(proportionOfHeight(0.65f)));
}
