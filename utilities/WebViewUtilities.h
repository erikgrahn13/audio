#pragma once
#include <juce_core/juce_core.h>
#include <vector>

#if defined NDEBUG
static std::vector<std::byte> streamToVector(juce::InputStream &stream)
{
    using namespace juce;
    const auto sizeInBytes = static_cast<size_t>(stream.getTotalLength());
    std::vector<std::byte> result(sizeInBytes);
    stream.setPosition(0);
    [[maybe_unused]] const auto bytesRead = stream.read(result.data(), result.size());
    jassert(bytesRead == static_cast<ssize_t>(sizeInBytes));
    return result;
}

#endif

static std::vector<std::byte> getWebViewFileAsBytes(const juce::String &filepath)
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

static const char *getMimeForExtension(const juce::String &extension)
{
    static const std::unordered_map<juce::String, const char *> mimeMap = {{{"htm"}, "text/html"},
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