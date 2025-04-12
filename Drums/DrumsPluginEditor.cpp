#include "DrumsPluginEditor.h"
#include "KickSamples.h"
#include "SnareSamples.h"
#include "DrumsPluginProcessor.h"
#include <JuceHeader.h>
#if defined NDEBUG
#include <WebViewFiles.h>
#endif
#include "WebViewUtilities.h"



//==============================================================================
DrumsAudioProcessorEditor::DrumsAudioProcessorEditor(DrumsAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p),
      webViewComponent{juce::WebBrowserComponent::Options{}
                           .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
                           .withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}.withUserDataFolder(
                               juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)))
                           .withNativeIntegrationEnabled()
                           .withInitialisationData("kickSamples", [this]() {
                            juce::Array<juce::var> sampleVarList;
                            for (int i = 0; i < KickSamples::namedResourceListSize; ++i)
                            sampleVarList.add(KickSamples::namedResourceList[i]);
                            return juce::var(sampleVarList);
                        }())
                        .withInitialisationData("snareSamples", [this]() {
                            juce::Array<juce::var> sampleVarList;
                            for (int i = 0; i < SnareSamples::namedResourceListSize; ++i)
                            sampleVarList.add(SnareSamples::namedResourceList[i]);
                            return juce::var(sampleVarList);
                        }())
                          .withNativeFunction(juce::Identifier{"nativeFunction"},
                                               [this](const juce::Array<juce::var> &args,
                                                      juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                                                   nativeFunction(args, std::move(completion));
                                               })
                           .withResourceProvider([this](const auto &url) { return getResource(url); },
                                                 juce::URL{"http://localhost:5500/"}.getOrigin())}
{
    juce::ignoreUnused(processorRef);

#if !defined NDEBUG
    webViewComponent.goToURL("http://127.0.0.1:5500/build/Drums/ui/index.html");
#else
    webViewComponent.goToURL(WebViewComponent::getResourceProviderRoot());
#endif
    addAndMakeVisible(webViewComponent);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);
    setSize(400, 300);
}

DrumsAudioProcessorEditor::~DrumsAudioProcessorEditor()
{
}

//==============================================================================

void DrumsAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getBounds();

    webViewComponent.setBounds(bounds);
}

void DrumsAudioProcessorEditor::nativeFunction(const juce::Array<juce::var> &args,
                                               juce::WebBrowserComponent::NativeFunctionCompletion completion)
{
    // std::ignore = args;
    std::ignore = completion;
    int hej = 2;
    hej = 3;

    const int drumType = args[0].toString().getIntValue();
    const int index = args[1].toString().getIntValue();

    processorRef.updatePreviewSample(drumType, index);
}

std::optional<juce::WebBrowserComponent::Resource> DrumsAudioProcessorEditor::getResource(const juce::String &url)
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
