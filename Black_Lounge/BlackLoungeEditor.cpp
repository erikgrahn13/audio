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
                                    juce::URL{"http://localhost:5500/"}.getOrigin())}

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

#if !defined NDEBUG
    webBrowserComponent.goToURL("http://127.0.0.1:5500/build/Black_Lounge/ui/index.html");
#else
    webBrowserComponent.goToURL(WebBrowserComponent::getResourceProviderRoot());
#endif

    addAndMakeVisible(webBrowserComponent);

    setResizable(true, false);
    setSize(480, 480);
    setResizeLimits(480, 480, 480, 480); // Lock dimensions

    // auto sampleRate = processorRef.getSampleRate();
    // mPitchMPM = std::make_unique<PitchMPM>(static_cast<int>(processorRef.getSampleRate()), 1024);
    startTimerHz(30);
    // mPitchMPM->setBufferSize(1024);
    // mPitchMPM->setSampleRate(processorRef.getSampleRate());

    // if (juce::JUCEApplicationBase::isStandaloneApp())
    // {
    //     settingsButton.setLookAndFeel(&fontWebLookAndFeel);
    //     settingsButton.setButtonText(juce::CharPointer_UTF8(""));
    //     // settingsButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::red);
    //     addAndMakeVisible(settingsButton);
    //     settingsButton.onClick = [] { juce::StandalonePluginHolder::getInstance()->showAudioSettingsDialog(); };
    // }

    // analyzeButton.setButtonText("Analyze");
    // mAnalyzeAttachment.reset(new
    // juce::AudioProcessorValueTreeState::ButtonAttachment(processorRef.getVTSParameters(),
    //                                                                                   "analyze", analyzeButton));
    // addAndMakeVisible(analyzeButton);

    // titleLabel.setText("BLACK LOUNGE", juce::NotificationType::dontSendNotification);
    // titleLabel.setJustificationType(juce::Justification::centred);

    // auto fontOptions = juce::FontOptions(
    //     juce::Typeface::createSystemTypefaceFor(CustomFont::ArtDystopia_ttf, CustomFont::ArtDystopia_ttfSize));
    // fontOptions = fontOptions.withHeight(60.);
    // titleLabel.setFont(juce::Font(fontOptions));
    // // titleLabel.setFont(juce::FontOptions(
    // //     juce::Typeface::createSystemTypefaceFor(CustomFont::ArtDystopia_ttf, CustomFont::ArtDystopia_ttfSize)));
    // // titleLabel.setFont(juce::Font(juce::FontOptions(
    // //     juce::Typeface::createSystemTypefaceFor(CustomFont::ArtDystopia_ttf,
    // //     CustomFont::ArtDystopia_ttfSize), 30.f, juce::Font::plain)));

    // // Apply the Font to your Label

    // // titleLabel.setLookAndFeel(&fontDeathMetalLookAndFeel);
    // addAndMakeVisible(titleLabel);

    // auto volumeName = processorRef.getVTSParameters().getParameter("volume")->getName(16).toUpperCase();
    // volumeLabel.setText(volumeName, juce::NotificationType::dontSendNotification);
    // volumeLabel.setJustificationType(juce::Justification::centred);
    // volumeLabel.setLookAndFeel(&fontDeathMetalLookAndFeel);
    // volumeLabel.attachToComponent(&volumeSlider, false);
    // volumeSlider.setLookAndFeel(&fontDeathMetalLookAndFeel);
    // volumeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    // volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    // mVolumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(processorRef.getVTSParameters(),
    //                                                                                  "volume", volumeSlider));
    // addAndMakeVisible(volumeSlider);

    // auto denoiserName = processorRef.getVTSParameters().getParameter("denoiser")->getName(16).toUpperCase();
    // denoiserLabel.setText(denoiserName, juce::NotificationType::dontSendNotification);
    // denoiserLabel.setJustificationType(juce::Justification::centred);
    // denoiserLabel.setLookAndFeel(&fontDeathMetalLookAndFeel);
    // denoiserLabel.attachToComponent(&denoiserSlider, false);
    // denoiserSlider.setLookAndFeel(&fontDeathMetalLookAndFeel);
    // denoiserSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    // denoiserSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    // mDenoiserAttachment.reset(new
    // juce::AudioProcessorValueTreeState::SliderAttachment(processorRef.getVTSParameters(),
    //                                                                                    "denoiser", denoiserSlider));

    // addAndMakeVisible(denoiserSlider);

    // auto gainName = processorRef.getVTSParameters().getParameter("gain")->getName(16).toUpperCase();
    // gainLabel.setText(gainName, juce::NotificationType::dontSendNotification);
    // gainLabel.setJustificationType(juce::Justification::centred);
    // gainLabel.setLookAndFeel(&fontDeathMetalLookAndFeel);
    // gainLabel.attachToComponent(&gainSlider, false);
    // gainSlider.setLookAndFeel(&fontDeathMetalLookAndFeel);
    // gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    // gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    // mGainAttachment.reset(
    //     new juce::AudioProcessorValueTreeState::SliderAttachment(processorRef.getVTSParameters(), "gain",
    //     gainSlider));

    // addAndMakeVisible(gainSlider);

    // mDenoiserActiveAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
    //     processorRef.getVTSParameters(), "denoiserActive", mDenoiserActiveButton));

    // mDenoiserActiveButton.setLookAndFeel(&fontWebLookAndFeel);
    // mDenoiserActiveButton.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::white);
    // mDenoiserActiveButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    // mDenoiserActiveButton.setButtonText(juce::CharPointer_UTF8(""));
    // mDenoiserActiveButton.onClick = [this]() {
    //     if (mDenoiserActiveButton.getToggleState())
    //     {
    //         mDenoiserActiveButton.setButtonText(juce::CharPointer_UTF8(""));
    //     }
    //     else
    //     {
    //         mDenoiserActiveButton.setButtonText(juce::CharPointer_UTF8(""));
    //     }
    // };
    // mDenoiserActiveButton.setClickingTogglesState(true);
    // mDenoiserActiveButton.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    // mDenoiserActiveButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::transparentBlack);
    // mDenoiserActiveButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);

    // addAndMakeVisible(mDenoiserActiveButton);

    // auto imageInputStream =
    //     juce::MemoryInputStream(BlackLoungeAmp::background1_png, BlackLoungeAmp::background1_pngSize, false);
    // backgroundImage = juce::ImageFileFormat::loadFrom(imageInputStream);
}

BlackLoungeAudioProcessorEditor::~BlackLoungeAudioProcessorEditor()
{
    denoiserLabel.setLookAndFeel(nullptr);
    gainLabel.setLookAndFeel(nullptr);
    volumeLabel.setLookAndFeel(nullptr);
    gainSlider.setLookAndFeel(nullptr);
    volumeSlider.setLookAndFeel(nullptr);
    denoiserSlider.setLookAndFeel(nullptr);
}

void BlackLoungeAudioProcessorEditor::timerCallback()
{
    // while (processorRef.mRingBuffer->getNumReady() >= 1024)
    // {
    //     std::array<float, 1024> tempBuffer;
    //     processorRef.mRingBuffer->readFromFifo(tempBuffer.data(), 1024);
    //     auto frequency = mTuner.getPitch(tempBuffer.data());
    //     DBG(frequency);
    // }

    if (processorRef.mRingBuffer->getNumReady() >= 4096)
    {
        std::array<float, 4096> samples;
        processorRef.mRingBuffer->readFromFifo(samples.data(), 4096);

        float pitchHz = mTuner.detectPitchAutocorrelation(samples.data(), 4096, 48000.0f);
        DBG("Pitch: " << pitchHz << " Hz");
    }
}

//==============================================================================
// void BlackLoungeAudioProcessorEditor::paint(juce::Graphics &g)
// {
//     // (Our component is opaque, so we must completely fill the background with a solid colour)
//     // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

//     // g.setColour(juce::Colours::white);
//     // g.setFont(15.0f);
//     // g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
//     // g.setColour(juce::Colours::black);
//     auto imagebounds = getLocalBounds().toFloat();
//     auto bounds = getLocalBounds().toFloat();

//     if (backgroundImage.isValid())
//     {
//         // g.setOpacity(0.0f);
//         imagebounds.removeFromTop(imagebounds.getHeight() / 1 / 6);

//         g.drawImage(backgroundImage, imagebounds.removeFromTop(imagebounds.getHeight() * 2 / 3));
//         g.setColour(juce::Colours::black.withAlpha(0.25f)); // Adjust the alpha for darkness
//         g.fillRect(getLocalBounds());
//     }
//     // g.fillRect(getLocalBounds());
//     // g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);
// }

void BlackLoungeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor.

    // auto bounds = getLocalBounds();

    // titleLabel.setBounds(0, 10, bounds.getWidth(), 100);

    // denoiserSlider.setBounds(40, bounds.getCentreY(), 80, 80);
    // mDenoiserActiveButton.setBounds(denoiserSlider.getWidth() / 2 + 20,
    //                                 bounds.getCentreY() + denoiserSlider.getHeight(), 40, 40);

    // gainSlider.setBounds(bounds.getCentreX() - 40, bounds.getCentreY() + 40, 80, 80);
    // volumeSlider.setBounds(bounds.getWidth() - 120, bounds.getCentreY() + 40, 80, 80);

    // // auto knobsArea = bounds.removeFromBottom(bounds.getHeight() / 3);

    // // volumeSlider.setBounds(bounds.removeFromRight(bounds.getWidth() / 2).reduced(50));
    // // thresholdSlider.setBounds(bounds.reduced(50));

    // settingsButton.setBounds(bounds.getWidth() - 50, getHeight() - 50, 40, 40);

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