#include "AbyssPluginEditor.h"
#include "AbyssPluginProcessor.h"
#include "DemoUtilities.h"
#include <JuceHeader.h>
#if defined NDEBUG
#include <WebViewFiles.h>
#endif
//==============================================================================
AbyssAudioProcessorEditor::AbyssAudioProcessorEditor(AbyssAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p),
      webBrowserComponent{
          juce::WebBrowserComponent::Options{}
              .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
              .withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}.withUserDataFolder(
                  juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)))
              .withOptionsFrom(gainRelay)
              .withResourceProvider([this](const auto &url) { return getResource(url); },
                                    juce::URL{"http://localhost:5500/"}.getOrigin())}
{

    auto *gainParam = processorRef.mParameters.getParameter("gain");
    gainWebAttachment = std::make_unique<juce::WebSliderParameterAttachment>(*gainParam, gainRelay, nullptr);

#if !defined NDEBUG
    webBrowserComponent.goToURL("http://127.0.0.1:5500/build/Abyss/ui/index.html");
#else
    webBrowserComponent.goToURL(WebBrowserComponent::getResourceProviderRoot());
#endif

    addAndMakeVisible(webBrowserComponent);

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

std::vector<std::byte> streamToVector(juce::InputStream &stream)
{
    using namespace juce;
    const auto sizeInBytes = static_cast<size_t>(stream.getTotalLength());
    std::vector<std::byte> result(sizeInBytes);
    stream.setPosition(0);
    [[maybe_unused]] const auto bytesRead = stream.read(result.data(), result.size());
    jassert(bytesRead == static_cast<ssize_t>(sizeInBytes));
    return result;
}

std::vector<std::byte> getWebViewFileAsBytes(const juce::String &filepath)
{
    std::ignore = filepath;
#if defined NDEBUG
    juce::MemoryInputStream zipStream{webview_files::webview_files_zip, webview_files::webview_files_zipSize, false};
    juce::ZipFile zipFile{zipStream};

    if (auto *zipEntry = zipFile.getEntry(ZIPPED_FILES_PREFIX + filepath))
    {
        const std::unique_ptr<juce::InputStream> entryStream{zipFile.createStreamForEntry(*zipEntry)};

        if (entryStream == nullptr)
        {
            jassertfalse;
            return {};
        }

        return streamToVector(*entryStream);
    }
#endif

    return {};
}

static const char *getMimeForExtension(const String &extension)
{
    static const std::unordered_map<String, const char *> mimeMap = {{{"htm"}, "text/html"},
                                                                     {{"html"}, "text/html"},
                                                                     {{"txt"}, "text/plain"},
                                                                     {{"jpg"}, "image/jpeg"},
                                                                     {{"jpeg"}, "image/jpeg"},
                                                                     {{"svg"}, "image/svg+xml"},
                                                                     {{"ico"}, "image/vnd.microsoft.icon"},
                                                                     {{"json"}, "application/json"},
                                                                     {{"png"}, "image/png"},
                                                                     {{"css"}, "text/css"},
                                                                     {{"map"}, "application/json"},
                                                                     {{"js"}, "text/javascript"},
                                                                     {{"woff2"}, "font/woff2"}};

    if (const auto it = mimeMap.find(extension.toLowerCase()); it != mimeMap.end())
        return it->second;

    jassertfalse;
    return "";
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
