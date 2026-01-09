#include "DrumsPluginEditor.h"
#include "KickSamples.h"
#include "SnareSamples.h"
#include "DrumsPluginProcessor.h"
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
                           .withOptionsFrom(flipPhaseRelay)
                           .withInitialisationData("isStandalone", juce::JUCEApplicationBase::isStandaloneApp())
                           .withInitialisationData("kickSamples", []() {
                            juce::Array<juce::var> sampleVarList;
                            for (int i = 0; i < KickSamples::namedResourceListSize; ++i)
                            sampleVarList.add(KickSamples::namedResourceList[i]);
                            return juce::var(sampleVarList);
                        }())
                        .withInitialisationData("snareSamples", []() {
                            juce::Array<juce::var> sampleVarList;
                            for (int i = 0; i < SnareSamples::namedResourceListSize; ++i)
                            sampleVarList.add(SnareSamples::namedResourceList[i]);
                            return juce::var(sampleVarList);
                        }())
                        .withInitialisationData("loadedSample", [this]() {
                            juce::Array<juce::var> loadedSample;
                            loadedSample.add(processorRef.mDrumType);
                            loadedSample.add(processorRef.mDrumIndex);
                            
                            return juce::var(loadedSample);
                        }())
                          .withNativeFunction(juce::Identifier{"playPreviewSample"},
                                               [this](const juce::Array<juce::var> &args,
                                                      juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                                                   playPreviewSample(args, std::move(completion));
                                               })
                            .withNativeFunction(juce::Identifier{"loadDrumSample"},
                                               [this](const juce::Array<juce::var> &args,
                                                      juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                                                   loadDrumSample(args, std::move(completion));
                                               })
                            .withNativeFunction(juce::Identifier{"clearSample"},
                                                [this](const juce::Array<juce::var> &args,
                                                    juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                                                    clearSample(args, std::move(completion));
                                               })
                            .withNativeFunction(juce::Identifier{"openSettingsView"},
                                                                           [this](const juce::Array<juce::var> &args,
                                                      juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                                                   openSettingsView(args, std::move(completion));
                                               })
                           .withResourceProvider([this](const auto &url) { return getResource(url); },
                                                 juce::URL{"http://localhost:5173/"}.getOrigin())}
{
    juce::ignoreUnused(processorRef);

    auto *flipPhaseParam = processorRef.apvts.getParameter("flipPhase");
    flipPhaseAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(*flipPhaseParam, flipPhaseRelay, nullptr);

#if !defined NDEBUG
    webViewComponent.goToURL("http://localhost:5173/");
#else
    webViewComponent.goToURL(juce::WebBrowserComponent::getResourceProviderRoot());
#endif
    addAndMakeVisible(webViewComponent);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, false);
    setSize(500, 560);
    setResizeLimits(500, 560, 500, 560);
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

void DrumsAudioProcessorEditor::openSettingsView(const juce::Array<juce::var> &args,
                                                 juce::WebBrowserComponent::NativeFunctionCompletion completion)
{
    std::ignore = args;
    std::ignore = completion;
    juce::StandalonePluginHolder::getInstance()->showAudioSettingsDialog();
}

void DrumsAudioProcessorEditor::playPreviewSample(const juce::Array<juce::var> &args,
                                               juce::WebBrowserComponent::NativeFunctionCompletion completion)
{
    std::ignore = completion;
    const int drumType = args[0].toString().getIntValue();
    const int index = args[1].toString().getIntValue();

    processorRef.playPreviewSample(drumType, index);
}

void DrumsAudioProcessorEditor::loadDrumSample(const juce::Array<juce::var> &args,
                                           juce::WebBrowserComponent::NativeFunctionCompletion completion)
{
    std::ignore = completion;
    const int drumType = args[0].toString().getIntValue();
    const int index = args[1].toString().getIntValue();

    processorRef.loadDrumSample(drumType, index);
}

void DrumsAudioProcessorEditor::clearSample(const juce::Array<juce::var> &args,
                                           juce::WebBrowserComponent::NativeFunctionCompletion completion)
{
    std::ignore = args;
    std::ignore = completion;
    processorRef.clearSample();
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
