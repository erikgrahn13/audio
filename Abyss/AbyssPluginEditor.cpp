#include "AbyssPluginEditor.h"
#include "AbyssPluginProcessor.h"

//==============================================================================
AbyssAudioProcessorEditor::AbyssAudioProcessorEditor(AbyssAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p),
      webBrowserComponent{juce::WebBrowserComponent::Options{}.withResourceProvider(
          [this](const auto &url) { return getResource(url); }, juce::URL("http://localhost:5000").getOrigin())}
{
    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(webBrowserComponent);
    webBrowserComponent.goToURL(webBrowserComponent.getResourceProviderRoot());

    setSize(400, 300);
}

AbyssAudioProcessorEditor::~AbyssAudioProcessorEditor()
{
}

//==============================================================================
void AbyssAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AbyssAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getBounds();

    webBrowserComponent.setBounds(bounds);
}

std::optional<juce::WebBrowserComponent::Resource> AbyssAudioProcessorEditor::getResource(const juce::String &url)
{
    const auto urlToRetrive = url == "/" ? juce::String{"index.html"} : url.fromFirstOccurrenceOf("/", false, false);

    // std::cout << "erik: " << url << std::endl;
    DBG("erik:" << url);

    return std::nullopt;
}