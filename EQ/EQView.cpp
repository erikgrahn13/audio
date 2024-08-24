#include "EQView.h"
#include <numbers>

EQView::EQView(AudioPluginAudioProcessor &processor, juce::AudioProcessorValueTreeState &parameters)
    : mProcessor(processor), mParameters(parameters)
{

    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kHighpass, dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("hpf_freq"))));
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kLowpass, dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("lpf_freq"))));
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kLowShelf, dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("LowShelfFreq")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("LowShelfGain"))));
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kHighShelf, dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("HighShelfFreq")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("HighShelfGain"))));
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kPeak, dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("LowMidFreq")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("LowMidGain")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("LowMidQ"))));
    mHandles.push_back(std::make_unique<Handle>(
        Biquad::Type::kPeak, dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("HighMidFreq")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("HighMidGain")),
        dynamic_cast<juce::AudioParameterFloat *>(mParameters.getParameter("HighMidQ"))));

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

        addAndMakeVisible(*handle);
        handle->toFront(true);
    }

    mSampleRate = mProcessor.getSampleRate();
}

EQView::~EQView()
{
}

void EQView::resized()
{
    auto area = getLocalBounds();

    // Calculate the position so the center of the handle represents the parameter value
    for (auto &handle : mHandles)
    {

        float xPosition = area.getWidth() * handle->mFreqParameter->getValue();
        float yPosition = area.getHeight() / 2.0f; // Center Y position

        handle->setBounds(static_cast<int>(xPosition - Handle::handSize / 2),
                          static_cast<int>(yPosition - Handle::handSize / 2), Handle::handSize, Handle::handSize);
    }
}

void EQView::paint(juce::Graphics &g)
{

    drawGrid(g);

    drawPlotCurve(g);
    g.setOpacity(1.);
    g.setColour(juce::Colours::white);
    g.strokePath(frequencyResponse, juce::PathStrokeType(2.0));
}

std::vector<int> getFrequencies()
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

std::vector<float> getXs(const std::vector<double> &freqs, float left, float width)
{
    std::vector<float> xs;
    auto logMin = std::log10(20.f);
    auto logMax = std::log10(20000.f);

    for (auto f : freqs)
    {
        auto normX = (std::log10(f) - logMin) / (logMax - logMin);
        xs.push_back(left + width * normX);
    }

    return xs;
}

std::vector<int> getGains()
{
    return std::vector<int>{-18, -12, -6, 0, 6, 12, 18};
}

void EQView::drawVerticalLines(juce::Graphics &g)
{
    auto size = getLocalBounds();
    auto freqs = getFrequencies();

    auto xs = getXs(std::vector<double>{freqs.begin(), freqs.end()}, 0.0, size.getWidth() - 1);

    for (auto x : xs)
    {
        g.drawVerticalLine(x, size.getY(), size.getBottom());
    }
}

void EQView::drawHorizontalLines(juce::Graphics &g)
{
    auto size = getLocalBounds();
    auto gains = getGains();

    auto hej1 = size.getTopLeft().getX();
    auto hej2 = size.getTopRight().getX();

    for (auto gaindB : gains)
    {
        auto y = size.getHeight() - 1 + ((1 - size.getHeight()) * (gaindB - -18.0)) / (18.0 - -18.0);
        g.drawHorizontalLine(y, size.getTopLeft().getX(), size.getTopRight().getX());
    }
}

void EQView::drawPlotCurve(juce::Graphics &g)
{

    frequencyResponse.clear();

    float width = getLocalBounds().getWidth();
    float height = getLocalBounds().getHeight();
    auto bounds = getLocalBounds();

    const double outputMin = bounds.getBottom();
    const double outputMax = bounds.getY();
    const auto sampleRate = mProcessor.getSampleRate();

    if (mSampleRate != sampleRate)
    {
        for (auto &handle : mHandles)
        {
            handle->biquad.setSampleRate(sampleRate);
        }
        mSampleRate = sampleRate;
    }

    for (int i = 0; i < width; ++i)
    {
        float freq = i / width;
        double y = 0;

        for (auto &handle : mHandles)
        {
            y += Biquad::filterResponse(sampleRate, freq, handle->biquad);
        }

        y = juce::jmap(y, -18.0, 18.0, outputMin, outputMax);
        if (i == 0)
        {
            frequencyResponse.startNewSubPath(bounds.getX(), y);
        }

        frequencyResponse.lineTo(bounds.getX() + i, y);

        g.setColour(juce::Colours::grey.withAlpha(0.5f));
        g.drawLine(i, height / 2, i, y);
    }
}

void EQView::updateFrequencyResponse()
{
    repaint();
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

    constrainer.setMinimumOnscreenAmounts(Handle::handSize / 2, Handle::handSize / 2, Handle::handSize / 2,
                                          Handle::handSize / 2);
}

void EQView::Handle::paint(juce::Graphics &g)
{
    g.setColour(Colours::black);
    g.fillEllipse(juce::Rectangle<float>(handSize, handSize));

    g.setColour(Colours::white);

    juce::Point<float> point1 = getLocalBounds().getCentre().toFloat();
    point1.setX(point1.getX() + (handSize / 2) * std::sin(0. * std::numbers::pi / 180.));
    point1.setY(point1.getY() + (handSize / 2) * std::cos(0. * std::numbers::pi / 180.));

    juce::Point<float> point2 = getLocalBounds().getCentre().toFloat();
    point2.setX(point2.getX() + (handSize / 2) * std::sin(144. * std::numbers::pi / 180.));
    point2.setY(point2.getY() + (handSize / 2) * std::cos(144. * std::numbers::pi / 180.));

    juce::Point<float> point3 = getLocalBounds().getCentre().toFloat();
    point3.setX(point3.getX() + (handSize / 2) * std::sin(288. * std::numbers::pi / 180.));
    point3.setY(point3.getY() + (handSize / 2) * std::cos(288. * std::numbers::pi / 180.));

    juce::Point<float> point4 = getLocalBounds().getCentre().toFloat();
    point4.setX(point4.getX() + (handSize / 2) * std::sin(72. * std::numbers::pi / 180.));
    point4.setY(point4.getY() + (handSize / 2) * std::cos(72. * std::numbers::pi / 180.));

    juce::Point<float> point5 = getLocalBounds().getCentre().toFloat();
    point5.setX(point5.getX() + (handSize / 2) * std::sin(216. * std::numbers::pi / 180.));
    point5.setY(point5.getY() + (handSize / 2) * std::cos(216. * std::numbers::pi / 180.));

    juce::Path path;
    path.startNewSubPath(point1);
    path.lineTo(point2);
    path.lineTo(point3);
    path.lineTo(point4);
    path.lineTo(point5);
    path.closeSubPath();

    g.strokePath(path, juce::PathStrokeType(1));

    g.drawEllipse(juce::Rectangle<float>(handSize, handSize).reduced(2, 2), 1.);
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
    if (mQParameter)
    {
        float qValue = mQParameter->getValue() + wheel.deltaY;
        mQParameter->setValueNotifyingHost(qValue);
    }
}

void EQView::Handle::updateFrequencyPositionFromParameter(float newValue)
{
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
    if (auto *parent = getParentComponent())
    {
        auto area = parent->getLocalBounds();
        float yPosition = area.getHeight() - (mGainParameter->getValue() * area.getHeight());

        setTopLeftPosition(getX(), static_cast<int>(yPosition - getHeight() / 2));
        repaint();
    }
}
