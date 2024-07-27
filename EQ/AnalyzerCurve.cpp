#include "AnalyzerCurve.h"

AnalyzerCurve::AnalyzerCurve(AudioPluginAudioProcessor &processor) : mProcessor(processor)
{
    avgFFTBuffer.clear();

    mFFTPoints.resize(mFFT.getSize());
    {
        juce::ScopedLock lockedForWriting(pathCreationLock);
        mFFTPath.preallocateSpace(mFFT.getSize() * 2);
    }

    startTimerHz(30);
}

AnalyzerCurve::~AnalyzerCurve()
{
}

void AnalyzerCurve::paint(juce::Graphics &g)
{
    const auto bounds = getLocalBounds().toFloat();
    const auto width = bounds.getWidth();
    const auto height = bounds.getHeight();

    const auto *fftDataOutput = avgFFTBuffer.getReadPointer(0);

    juce::ScopedLock lockedForReading(pathCreationLock);
    mFFTPath.clear();

    {
        FFTPoint &point = mFFTPoints[0];
        float y = juce::jmap(getFFTPointLevel(fftDataOutput, point), mindB, maxdB, height, 0.0f) + 0.5f;
        mFFTPath.startNewSubPath(float(point.x), y);
    }

    for (int i = 0; i < fftPointsSize; ++i)
    {
        FFTPoint &point = mFFTPoints[i];
        float y = juce::jmap(getFFTPointLevel(fftDataOutput, point), mindB, maxdB, height, 0.0f) + 0.5f;
        mFFTPath.lineTo(float(point.x), y);
    }

    mFFTPath.lineTo(mFFTPath.getCurrentPosition().getX(), height); // Go to the bottom of the last x point
    mFFTPath.lineTo(0, height);                                    // Go to the bottom-left corner

    mFFTPath.closeSubPath();
    g.setColour(juce::Colours::grey.withAlpha(0.25f));
    g.fillPath(mFFTPath);

    g.setColour(juce::Colours::white.withAlpha(0.25f));
    g.strokePath(mFFTPath, juce::PathStrokeType(0.8f));
}

void AnalyzerCurve::resized()
{
    const auto bounds = getLocalBounds();
    auto widthFactor = bounds.getWidth() / 10.0f;
    auto sampleRate = float(mProcessor.getSampleRate());
    auto fftSize = mFFT.getSize();

    fftPointsSize = 0;
    int lastX = 0;
    mFFTPoints[0].firstBinIndex = 0;

    int i = 0;
    while (i < fftSize)
    {
        FFTPoint &point = mFFTPoints[fftPointsSize];
        point.firstBinIndex = i;
        point.x = lastX;

        int x = lastX;
        while ((x <= lastX) && (i < fftSize))
        {
            ++i;

            auto pos = std::log(((sampleRate * i) / fftSize) / 20.f) / std::log(2.0f);
            x = juce::roundToInt((pos > 0.0f) ? (widthFactor * pos) + 0.5f : 0);
        }

        point.lastBinIndex = i - 1;

        ++fftPointsSize;
        lastX = x;
    }
}

void AnalyzerCurve::timerCallback()
{
    if (!mProcessor.nextFFTBlockReady.load())
        return;

    drawNextFrame();
    mProcessor.nextFFTBlockReady.store(false);
    repaint();
}

float AnalyzerCurve::getFFTPointLevel(const float *buffer, const FFTPoint &point)
{
    float level = 0.0f;

    for (int i = point.firstBinIndex; i <= point.lastBinIndex; ++i)
    {
        if (buffer[i] > level)
            level = buffer[i];
    }

    return juce::Decibels::gainToDecibels(level, mindB);
}

void AnalyzerCurve::drawNextFrame()
{
    while (mProcessor.mRingBuffer.getNumReady() >= mFFT.getSize())
    {
        mFFTBuffer.clear();

        int start1, block1, start2, block2;
        mProcessor.mRingBuffer.prepareToRead(mFFT.getSize(), start1, block1, start2, block2);

        if (block1 > 0)
            mFFTBuffer.copyFrom(0, 0, mProcessor.mAudioBuffer.getReadPointer(0, start1), block1);
        if (block2 > 0)
            mFFTBuffer.copyFrom(0, block1, mProcessor.mAudioBuffer.getReadPointer(0, start2), block2);

        mProcessor.mRingBuffer.finishedRead((block1 + block2) / 2);

        mWindow.multiplyWithWindowingTable(mFFTBuffer.getWritePointer(0), size_t(mFFT.getSize()));
        mFFT.performFrequencyOnlyForwardTransform(mFFTBuffer.getWritePointer(0));

        juce::ScopedLock lockedForWriting(pathCreationLock);
        avgFFTBuffer.addFrom(0, 0, avgFFTBuffer.getReadPointer(avgFFTBufferPtr), avgFFTBuffer.getNumSamples(), -1.0f);
        avgFFTBuffer.copyFrom(avgFFTBufferPtr, 0, mFFTBuffer.getReadPointer(0), avgFFTBuffer.getNumSamples(),
                              1.0f / (avgFFTBuffer.getNumSamples() * (avgFFTBuffer.getNumChannels() - 1)));
        avgFFTBuffer.addFrom(0, 0, avgFFTBuffer.getReadPointer(avgFFTBufferPtr), avgFFTBuffer.getNumSamples());

        if (++avgFFTBufferPtr == avgFFTBuffer.getNumChannels())
            avgFFTBufferPtr = 1;
    }
}
