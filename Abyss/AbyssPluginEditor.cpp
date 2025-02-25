#include "AbyssPluginEditor.h"
#include "AbyssPluginProcessor.h"

//==============================================================================
AbyssAudioProcessorEditor::AbyssAudioProcessorEditor(AbyssAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p),
      webBrowserComponent{
          juce::WebBrowserComponent::Options{}
              .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
              .withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}.withUserDataFolder(
                  juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)))
              .withOptionsFrom(gainRelay)}
{
    juce::ignoreUnused(processorRef);
    juce::File editorSourceFile = juce::File(__FILE__);
    juce::File uiDirectory = editorSourceFile.getParentDirectory().getChildFile("ui");
    auto indexFile = uiDirectory.getChildFile("index.html");

    auto *gainParam = processorRef.mParameters.getParameter("gain");
    gainWebAttachment = std::make_unique<juce::WebSliderParameterAttachment>(*gainParam, gainRelay, nullptr);

    if (indexFile.existsAsFile())
    {
        webBrowserComponent.goToURL("http://127.0.0.1:5500/Abyss/ui/index.html");
    }
    else
    {
        juce::Logger::writeToLog("UI file not found: " + indexFile.getFullPathName());
        webBrowserComponent.goToURL("data:text/html,<html><body><h1>UI File Not Found</h1></body></html>");
    }

    addAndMakeVisible(webBrowserComponent);

    setSize(400, 300);
}

AbyssAudioProcessorEditor::~AbyssAudioProcessorEditor()
{
}

//==============================================================================
// void AbyssAudioProcessorEditor::paint(juce::Graphics &g)
// {
//     std::ignore = g;
//     // (Our component is opaque, so we must completely fill the background with a solid colour)
//     // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

//     // g.setColour(juce::Colours::white);
//     // g.setFont(15.0f);
//     // g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
// }

void AbyssAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getBounds();

    webBrowserComponent.setBounds(bounds);
}
