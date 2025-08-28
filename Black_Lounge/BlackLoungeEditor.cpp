#include "BlackLoungeEditor.h"
#include "BlackLoungeAmp.h"
#include "BlackLoungeProcessor.h"
#include <JuceHeader.h>
#if defined NDEBUG
#include <WebViewFiles.h>
#endif
#include "WebViewUtilities.h"

//==============================================================================
BlackLoungeAudioProcessorEditor::BlackLoungeAudioProcessorEditor(BlackLoungeAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p),
      webBrowserComponent{
          juce::WebBrowserComponent::Options{}
              .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
              .withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}.withUserDataFolder(
                  juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)))
              .withNativeIntegrationEnabled()
              .withInitialisationData("isStandalone", juce::JUCEApplicationBase::isStandaloneApp())
              .withOptionsFrom(volumeRelay)
              .withOptionsFrom(gainRelay)
              .withOptionsFrom(denoiserRelay)
              .withOptionsFrom(denoiserActiveRelay)
              .withNativeFunction(juce::Identifier{"nativeFunction"},
                                  [this](const juce::Array<juce::var> &args,
                                         juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                                      nativeFunction(args, std::move(completion));
                                  })
              .withResourceProvider([this](const auto &url) { return getResource(url); },
                                    juce::URL{"http://localhost:5173/"}.getOrigin())}

{
    juce::ignoreUnused(processorRef);

    auto *volumeParam = processorRef.mParameters.getParameter("volume");
    auto *gainParam = processorRef.mParameters.getParameter("gain");
    auto *denoiserParam = processorRef.mParameters.getParameter("denoiser");
    auto *denoiserActiveParam = processorRef.mParameters.getParameter("denoiserActive");
    volumeWebAttachment = std::make_unique<juce::WebSliderParameterAttachment>(*volumeParam, volumeRelay, nullptr);
    gainWebAttachment = std::make_unique<juce::WebSliderParameterAttachment>(*gainParam, gainRelay, nullptr);
    denoiserWebAttachment =
        std::make_unique<juce::WebSliderParameterAttachment>(*denoiserParam, denoiserRelay, nullptr);
    denoiserActiveAttachment =
        std::make_unique<juce::WebToggleButtonParameterAttachment>(*denoiserActiveParam, denoiserActiveRelay, nullptr);

    currentSampleRate = processorRef.getSampleRate();

#if !defined NDEBUG
    webBrowserComponent.goToURL("http://127.0.0.1:5173/build/Black_Lounge/ui/index.html");
#else
    webBrowserComponent.goToURL(WebBrowserComponent::getResourceProviderRoot());
#endif

    addAndMakeVisible(webBrowserComponent);

    setResizable(true, false);
    setSize(480, 540);
    setResizeLimits(480, 540, 480, 540); // Lock dimensions

    startTimerHz(30);

    if (juce::JUCEApplicationBase::isStandaloneApp())
    {
        if (auto *pluginHolder = juce::StandalonePluginHolder::getInstance())
        {
            pluginHolder->getMuteInputValue().setValue(false);
        }
    }
}

BlackLoungeAudioProcessorEditor::~BlackLoungeAudioProcessorEditor()
{
}

void BlackLoungeAudioProcessorEditor::timerCallback()
{
    if (processorRef.mRingBuffer->getNumReady() >= 4096)
    {
        std::array<float, 4096> samples;
        processorRef.mRingBuffer->readFromFifo(samples.data(), 4096);

        float pitchHz = Tuner::detectPitchAutocorrelation(samples.data(), 4096, currentSampleRate);
        webBrowserComponent.emitEventIfBrowserIsVisible(juce::Identifier("pitch"), pitchHz);
    }
}

void BlackLoungeAudioProcessorEditor::resized()
{
    auto bounds = getBounds();
    webBrowserComponent.setBounds(bounds);
}

std::optional<juce::WebBrowserComponent::Resource> BlackLoungeAudioProcessorEditor::getResource(const juce::String &url)
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

void BlackLoungeAudioProcessorEditor::nativeFunction(const juce::Array<juce::var> &args,
                                                     juce::WebBrowserComponent::NativeFunctionCompletion completion)
{
    std::ignore = args;
    std::ignore = completion;
    juce::StandalonePluginHolder::getInstance()->showAudioSettingsDialog();
}