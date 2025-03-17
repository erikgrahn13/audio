#include "AbyssPluginEditor.h"
#include "AbyssPluginProcessor.h"
#include "DemoUtilities.h"
#include <JuceHeader.h>

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

    // #if !defined NDEBUG
    juce::ignoreUnused(processorRef);
    juce::File editorSourceFile = juce::File(__FILE__);
    juce::File uiDirectory = editorSourceFile.getParentDirectory().getChildFile("ui");
    auto indexFile = uiDirectory.getChildFile("index.html");

    if (indexFile.existsAsFile())
    {
        webBrowserComponent.goToURL("http://127.0.0.1:5500/Abyss/ui/index.html");
    }
    else
    {
        juce::Logger::writeToLog("UI file not found: " + indexFile.getFullPathName());
        webBrowserComponent.goToURL("data:text/html,<html><body><h1>UI File Not Found</h1></body></html>");
    }
    // #else
    // webBrowserComponent.goToURL(WebBrowserComponent::getResourceProviderRoot());

    // #endif

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

// static auto streamToVector(InputStream &stream)
// {
//     std::vector<std::byte> result((size_t)stream.getTotalLength());
//     stream.setPosition(0);
//     [[maybe_unused]] const auto bytesRead = stream.read(result.data(), result.size());
//     jassert(bytesRead == (ssize_t)result.size());
//     return result;
// }

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

static String getExtension(String filename)
{
    return filename.fromLastOccurrenceOf(".", false, false);
}

std::optional<juce::WebBrowserComponent::Resource> AbyssAudioProcessorEditor::getResource(const juce::String &url)
{
    const auto filename = url == "/" ? juce::String{"index.html"} : url.fromFirstOccurrenceOf("/", false, false);

    // if (auto *archive = getZipFile())
    // {
    //     if (auto *entry = archive->getEntry(urlToRetrive))
    //     {
    //         auto stream = rawToUniquePtr(archive->createStreamForEntry(*entry));
    //         auto v = streamToVector(*stream);
    //         auto mime = getMimeForExtension(getExtension(entry->filename).toLowerCase());
    //         return WebBrowserComponent::Resource{std::move(v), std::move(mime)};
    //     }
    // }
    const void *data{nullptr};
    int dataSize{0};

    if (filename == "index.html")
    {
        data = AbyssData::index_html;
        dataSize = AbyssData::index_htmlSize;
        // data = Abyss::auto fallbackIndexHtml = createAssetInputStream("webviewplugin-gui-fallback.html");
        // return WebBrowserComponent::Resource{streamToVector(*fallbackIndexHtml), String{"text/html"}};
    }

    // if (urlToRetrive == "DeathMetalKnob.js")
    // {
    //     auto fallbackIndexHtml = createAssetInputStream("webviewplugin-gui-fallback.html");
    //     return WebBrowserComponent::Resource{streamToVector(*fallbackIndexHtml), String{"text/html"}};
    // }

    // if (urlToRetrive == "data.txt")
    // {
    //     WebBrowserComponent::Resource resource;
    //     static constexpr char testData[] = "testdata";
    //     MemoryInputStream stream{testData, numElementsInArray(testData) - 1, false};
    //     return WebBrowserComponent::Resource{streamToVector(stream), String{"text/html"}};
    // }

    // if (urlToRetrive == "spectrumData.json")
    // {
    //     Array<var> frames;

    //     for (const auto &frame : spectrumDataFrames)
    //         frames.add(frame);

    //     DynamicObject::Ptr d(new DynamicObject());
    //     d->setProperty("timeResolutionMs", getTimerInterval());
    //     d->setProperty("frames", std::move(frames));

    //     const auto s = JSON::toString(d.get());
    //     MemoryInputStream stream{s.getCharPointer(), s.getNumBytesAsUTF8(), false};
    //     return WebBrowserComponent::Resource{streamToVector(stream), String{"application/json"}};
    // }

    auto mimeType = getMimeForExtension(getExtension(filename));

    return WebBrowserComponent::Resource{std::vector<std::byte>((std::byte *)data, (std::byte *)data + dataSize),
                                         String(mimeType)};

    // return std::nullopt;
}
