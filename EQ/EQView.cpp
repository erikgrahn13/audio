#include "EQView.h"
#include <cmath>
#include <numbers>

EQView::EQView(AudioPluginAudioProcessor &processor, juce::AudioProcessorValueTreeState &parameters)
    : mProcessor(processor), mParameters(parameters), mAnalyzerCurve(processor),
      deathMetalFont(juce::FontOptions(
          juce::Typeface::createSystemTypefaceFor(BinaryData::ArtDystopia_ttf, BinaryData::ArtDystopia_ttfSize)))
{
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kHighpass, dynamic_cast<CustomAudioParameterFloat *>(mParameters.getParameter("hpf_freq"))));
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kLowpass, dynamic_cast<CustomAudioParameterFloat *>(mParameters.getParameter("lpf_freq"))));
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kLowShelf, dynamic_cast<CustomAudioParameterFloat *>(mParameters.getParameter("LowShelfFreq")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("LowShelfGain"))));
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kHighShelf, dynamic_cast<CustomAudioParameterFloat *>(mParameters.getParameter("HighShelfFreq")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("HighShelfGain"))));
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kPeak, dynamic_cast<CustomAudioParameterFloat *>(mParameters.getParameter("LowMidFreq")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("LowMidGain")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("LowMidQ"))));
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kPeak, dynamic_cast<CustomAudioParameterFloat *>(mParameters.getParameter("HighMidFreq")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("HighMidGain")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("HighMidQ"))));

    addAndMakeVisible(mAnalyzerCurve);
    addAndMakeVisible(handleContainer);

    for (auto &handle : mHandles)
    {
        handle->biquad.setSampleRate(mProcessor.getSampleRate());
        handle->biquad.setFrequency(handle->mFreqParameter->convertFrom0to1(handle->mFreqParameter->getValue()));

        if (handle->mGainParameter)
        {
            handle->biquad.setGain(handle->mGainParameter->convertFrom0to1(handle->mGainParameter->getValue()));
        }

        if (handle->mQParameter)
        {
            handle->biquad.setQ(handle->mQParameter->convertFrom0to1(handle->mQParameter->getValue()));
        }

        handleContainer.addAndMakeVisible(handle.get());
        handle->toFront(true);
    }

    mSampleRate = static_cast<int>(mProcessor.getSampleRate());
}

EQView::~EQView()
{
}

void EQView::resized()
{
    auto area = getRenderArea();

    mAnalyzerCurve.setBounds(area);
    handleContainer.setBounds(area);

    // Calculate the position so the center of the handle represents the parameter value
    for (auto &handle : mHandles)
    {

        float xPosition = handleContainer.getLocalBounds().getWidth() * handle->mFreqParameter->getValue();
        float yPosition = handleContainer.getLocalBounds().getCentreY(); // Center Y position

        handle->setBounds(static_cast<int>(xPosition - Handle::handleSize / 2),
                          static_cast<int>(yPosition - Handle::handleSize / 2), Handle::handleSize, Handle::handleSize);
    }
}

void EQView::paint(juce::Graphics &g)
{
    drawGrid(g);
    drawTextLabels(g);

    g.saveState();

    drawPlotCurve(g);
    g.setOpacity(1.);
    g.setColour(juce::Colours::white);
    g.strokePath(frequencyResponse, juce::PathStrokeType(2.0));

    g.restoreState();

    for (auto &handle : mHandles)
    {
        std::string str = std::to_string(dynamic_cast<CustomAudioParameterFloat *>(handle->mFreqParameter)->getIndex());
        Rectangle<int> r;

        auto pos = handle->getPosition();
        r.setBounds(pos.getX() + reducedSize, pos.getY() + reducedSize / 3, 20, 20);

        g.drawFittedText(str, r, juce::Justification::centred, 1);
    }
}

std::vector<int> EQView::getFrequencies()
{
    return std::vector<int>{20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000};
}

void EQView::drawGrid(juce::Graphics &g)
{
    g.setColour(juce::Colours::white);
    g.setOpacity(0.5);
    drawVerticalLines(g);
    drawHorizontalLines(g);
}

std::vector<float> EQView::getXs(const std::vector<float> &freqs, float left, float width)
{
    std::vector<float> xs;
    auto logMin = std::log10(20.f);
    auto logMax = std::log10(20000.f);

    for (auto f : freqs)
    {
        auto normX = (::log10f(f) - logMin) / (logMax - logMin);
        xs.push_back(left + width * normX);
    }

    return xs;
}

std::vector<int> EQView::getGains()
{
    return std::vector<int>{-18, -12, -6, 0, 6, 12, 18};
}

void EQView::drawTextLabels(juce::Graphics &g)
{
    g.setColour(juce::Colours::white);
    auto freqs = getFrequencies();
    auto size = getRenderArea();

    g.setFont(deathMetalFont);

    auto xs = getXs(std::vector<float>{freqs.begin(), freqs.end()}, size.getX(), size.getWidth() - 1);

    for (size_t i = 0; i < freqs.size(); ++i)
    {
        bool addK = false;
        std::string str;
        auto freq = freqs[i];

        if (freq > 999.f)
        {
            addK = true;
            freq /= 1000;
        }

        str = std::to_string(freq);
        if (addK)
        {
            str.append("K");
        }

        auto textWidth = g.getCurrentFont().getStringWidth(str);
        Rectangle<int> r;

        r.setSize(textWidth, reducedSize);
        r.setCentre(static_cast<int>(xs[i]), 0);
        r.setY(size.getBottom());
        g.drawFittedText(str, r, juce::Justification::centred, 1);
    }

    auto gainsdB = getGains();

    for (auto gaindB : gainsdB)
    {
        auto y = jmap(static_cast<float>(gaindB), -18.f, 18.f, static_cast<float>(size.getBottom()),
                      static_cast<float>(size.getTopLeft().getY()));
        std::string str;
        if (gaindB > 0.f)
        {
            str = "+" + std::to_string(gaindB);
        }
        else
        {
            str = std::to_string(gaindB);
        }

        Rectangle<int> r;

        r.setSize(reducedSize, reducedSize);
        r.setX(getX());
        r.setCentre(r.getCentreX(), static_cast<int>(y));
        g.drawFittedText(str, r, juce::Justification::centred, 1);
    }
}

void EQView::drawVerticalLines(juce::Graphics &g)
{
    auto size = getRenderArea();
    auto freqs = getFrequencies();

    auto xs = getXs(std::vector<float>{freqs.begin(), freqs.end()}, 0.0, size.getWidth() - 1);

    for (auto x : xs)
    {
        g.drawVerticalLine(size.getX() + static_cast<int>(x), size.getY(), size.getBottom());
    }
}

void EQView::drawHorizontalLines(juce::Graphics &g)
{
    auto size = getRenderArea();
    auto gains = getGains();

    for (auto gaindB : gains)
    {
        auto y = size.getHeight() - 1 + ((1 - size.getHeight()) * (gaindB - -18.0)) / (18.0 - -18.0);
        g.drawHorizontalLine(size.getY() + static_cast<int>(y), size.getTopLeft().getX(), size.getTopRight().getX());
    }
}

void EQView::drawPlotCurve(juce::Graphics &g)
{
    frequencyResponse.clear();

    float width = getRenderArea().getWidth();
    auto bounds = getRenderArea();

    const float outputMin = bounds.getBottom();
    const float outputMax = bounds.getY();
    const auto sampleRate = static_cast<int>(mProcessor.getSampleRate());

    if (mSampleRate != sampleRate)
    {
        for (auto &handle : mHandles)
        {
            handle->biquad.setSampleRate(sampleRate);
        }
        mSampleRate = sampleRate;
    }

    g.reduceClipRegion(bounds);

    for (int i = 0; i < width; ++i)
    {
        float freq = i / width;
        float y = 0;

        for (auto &handle : mHandles)
        {
            y += static_cast<float>(Biquad::filterResponse(sampleRate, freq, handle->biquad));
        }

        y = juce::jmap(y, -18.f, 18.f, outputMin, outputMax);
        if (i == 0)
        {
            frequencyResponse.startNewSubPath(bounds.getX(), y);
        }

        frequencyResponse.lineTo(bounds.getX() + i, y);

        g.setColour(juce::Colours::grey.withAlpha(0.5f));
        g.drawLine(bounds.getX() + i, bounds.getCentreY(), bounds.getX() + i, y);
    }
}

void EQView::updateFrequencyResponse()
{
    repaint();
}

juce::Rectangle<int> EQView::getRenderArea()
{
    return getLocalBounds().reduced(reducedSize);
}

EQView::Handle::Handle(Biquad::Type type, juce::RangedAudioParameter *freqParam, juce::RangedAudioParameter *gainParam,
                       juce::RangedAudioParameter *qParam)
    : mFreqParameter(freqParam), mGainParameter(gainParam), mQParameter(qParam),
      biquad(type, mFreqParameter->getDefaultValue())

{
    mFreqAttachment = std::make_unique<juce::ParameterAttachment>(
        *mFreqParameter, [this](float newValue) { updateFrequencyPositionFromParameter(newValue); });
    mFreqAttachment->sendInitialUpdate();

    if (mGainParameter)
    {
        mGainAttachment = std::make_unique<juce::ParameterAttachment>(
            *mGainParameter, [this](float newValue) { updateGainPositionFromParameter(newValue); });
        mGainAttachment->sendInitialUpdate();
    }

    constrainer.setMinimumOnscreenAmounts(Handle::handleSize / 2, Handle::handleSize / 2, Handle::handleSize / 2,
                                          Handle::handleSize / 2);
}

void EQView::Handle::paint(juce::Graphics &g)
{
    g.setColour(Colours::black);
    g.fillEllipse(juce::Rectangle<float>(handleSize, handleSize));

    g.setColour(Colours::white);

    juce::Point<float> point1 = getLocalBounds().getCentre().toFloat();
    point1.setX(point1.getX() + (handleSize / 2) * ::sinf(0. * static_cast<float>(std::numbers::pi) / 180.));
    point1.setY(point1.getY() + (handleSize / 2) * ::cosf(0. * static_cast<float>(std::numbers::pi) / 180.));

    juce::Point<float> point2 = getLocalBounds().getCentre().toFloat();
    point2.setX(point2.getX() + (handleSize / 2) * ::sinf(144. * static_cast<float>(std::numbers::pi) / 180.));
    point2.setY(point2.getY() + (handleSize / 2) * ::cosf(144. * static_cast<float>(std::numbers::pi) / 180.));

    juce::Point<float> point3 = getLocalBounds().getCentre().toFloat();
    point3.setX(point3.getX() + (handleSize / 2) * ::sinf(288. * static_cast<float>(std::numbers::pi) / 180.));
    point3.setY(point3.getY() + (handleSize / 2) * ::cosf(288. * static_cast<float>(std::numbers::pi) / 180.));

    juce::Point<float> point4 = getLocalBounds().getCentre().toFloat();
    point4.setX(point4.getX() + (handleSize / 2) * ::sinf(72. * static_cast<float>(std::numbers::pi) / 180.));
    point4.setY(point4.getY() + (handleSize / 2) * ::cosf(72. * static_cast<float>(std::numbers::pi) / 180.));

    juce::Point<float> point5 = getLocalBounds().getCentre().toFloat();
    point5.setX(point5.getX() + (handleSize / 2) * ::sinf(216. * static_cast<float>(std::numbers::pi) / 180.));
    point5.setY(point5.getY() + (handleSize / 2) * ::cosf(216. * static_cast<float>(std::numbers::pi) / 180.));

    juce::Path path;
    path.startNewSubPath(point1);
    path.lineTo(point2);
    path.lineTo(point3);
    path.lineTo(point4);
    path.lineTo(point5);
    path.closeSubPath();

    g.strokePath(path, juce::PathStrokeType(1));

    g.drawEllipse(juce::Rectangle<float>(handleSize, handleSize).reduced(2, 2), 1.);
}

void EQView::Handle::mouseDown(const juce::MouseEvent &event)
{
    dragger.startDraggingComponent(this, event);
}

void EQView::Handle::mouseDrag(const juce::MouseEvent &event)
{
    int yPosition = getY();
    dragger.dragComponent(this, event, &constrainer);
    if (biquad.getType() == Biquad::Type::kHighpass || biquad.getType() == Biquad::Type::kLowpass)
    {
        setTopLeftPosition(getX(), yPosition);
    }

    // Update parameter value based on handle's center position
    float xValue = static_cast<float>(getX() + getWidth() / 2.0f) / getParentWidth();
    mFreqParameter->setValueNotifyingHost(xValue);

    if (mGainParameter)
    {
        float yValue = 1 - static_cast<float>(getY() + getHeight() / 2.0f) / getParentHeight();
        mGainParameter->setValueNotifyingHost(yValue);
    }
}

void EQView::Handle::mouseWheelMove(const juce::MouseEvent &event, const juce::MouseWheelDetails &wheel)
{
    std::ignore = event;
    if (mQParameter)
    {
        float qValue = mQParameter->getValue() + wheel.deltaY;
        mQParameter->setValueNotifyingHost(qValue);
    }
}

void EQView::Handle::updateFrequencyPositionFromParameter(float newValue)
{
    std::ignore = newValue;
    if (auto *parent = getParentComponent())
    {

        auto area = parent->getLocalBounds();
        float xPosition = area.getWidth() * mFreqParameter->getValue();

        setTopLeftPosition(static_cast<int>(xPosition - getWidth() / 2), getY());
        repaint();
    }
}

void EQView::Handle::updateGainPositionFromParameter(float newValue)
{
    std::ignore = newValue;
    if (auto *parent = getParentComponent())
    {
        auto area = parent->getLocalBounds();
        float yPosition = area.getHeight() - (mGainParameter->getValue() * area.getHeight());

        setTopLeftPosition(getX(), static_cast<int>(yPosition - getHeight() / 2));
        repaint();
    }
}
