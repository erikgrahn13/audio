#include "AbyssPluginEditor.h"
#include "AbyssPluginProcessor.h"
#include "DemoUtilities.h"
#include <JuceHeader.h>
#if defined NDEBUG
#include <WebViewFiles.h>
#endif
#include "WebViewUtilities.h"

//==============================================================================
AbyssAudioProcessorEditor::AbyssAudioProcessorEditor(AbyssAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p),
      webBrowserComponent{
          juce::WebBrowserComponent::Options{}
              .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
              .withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}.withUserDataFolder(
                  juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)))
              .withNativeIntegrationEnabled()
              .withOptionsFrom(gainRelay)
              .withOptionsFrom(bypassRelay)
              .withResourceProvider([this](const auto &url) { return getResource(url); },
                                    juce::URL{"http://localhost:5173/"}.getOrigin())}
{

    auto *gainParam = processorRef.mParameters.getParameter("gain");
    gainWebAttachment = std::make_unique<juce::WebSliderParameterAttachment>(*gainParam, gainRelay, nullptr);
    auto *bypassParam = processorRef.mParameters.getParameter("bypass");
    bypassWebAttachment =
        std::make_unique<juce::WebToggleButtonParameterAttachment>(*bypassParam, bypassRelay, nullptr);

#if !defined NDEBUG
    webBrowserComponent.goToURL("http://localhost:5173/");
#else
    webBrowserComponent.goToURL(WebBrowserComponent::getResourceProviderRoot());
#endif

    addAndMakeVisible(webBrowserComponent);
    setResizable(true, true);
    setSize(400, 300);
}

AbyssAudioProcessorEditor::~AbyssAudioProcessorEditor()
{
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
    const auto resourceToRetrieve =
        url == "/" ? juce::String{"index.html"} : url.fromFirstOccurrenceOf("/", false, false);

    const auto resource = getWebViewFileAsBytes(resourceToRetrieve);
    if (!resource.empty())
    {
        const auto extension = resourceToRetrieve.fromLastOccurrenceOf(".", false, false);
        return juce::WebBrowserComponent::Resource{std::move(resource), getMimeForExtension(extension)};
    }

    return std::nullopt;
}
