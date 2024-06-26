#include "AnalyzerCurve.h"

AnalyzerCurve::AnalyzerCurve(AudioPluginAudioProcessor& processor)
: mProcessor(processor)
{
    startTimerHz(60);

}

AnalyzerCurve::~AnalyzerCurve()
{
}

void AnalyzerCurve::timerCallback()
{
    if(!mProcessor.nextFFTBlockReady.load())
        return;
    updateBuffer();
    mProcessor.nextFFTBlockReady.store(false);
    repaint();
}

void AnalyzerCurve::paint(juce::Graphics &g)
{
        // g.fillAll (juce::Colours::black);
        g.setColour (juce::Colours::red);

        // Draw the audio waveform
        auto width = getWidth();
        auto height = getHeight();
        auto bufferData = audioSamples.getReadPointer(0);
        float scaleX = static_cast<float>(width) / (audioSamples.getNumSamples() - 1);
        float scaleY = height / 2.0f;

        juce::Path waveformPath;
        waveformPath.startNewSubPath(0, scaleY - bufferData[0] * scaleY);

        for (int i = 1; i < audioSamples.getNumSamples(); ++i)
        {
            waveformPath.lineTo(i * scaleX, scaleY - bufferData[i] * scaleY);
        }

        g.strokePath(waveformPath, juce::PathStrokeType(2.0f));
}

void AnalyzerCurve::updateBuffer()
{
    while(mProcessor.mRingBuffer.getNumReady() >= 4096)
    {
        audioSamples.clear();
        int start1, size1, start2, size2;
        mProcessor.mRingBuffer.prepareToRead(4096, start1, size1, start2, size2);
        if(size1 > 0)
            audioSamples.copyFrom(0, 0, mProcessor.mAudioBuffer.getReadPointer(0, start1), size1);
        if(size2 > 0)
            audioSamples.copyFrom(0, size1, mProcessor.mAudioBuffer.getReadPointer(0, start2), size2);


        // if(size1 > 0)
        //     audioSamples.copyFrom(0, 0, mProcessor.mAudioBuffer, 0, start1, size1);
        // if(size2 > 0)
        //     audioSamples.copyFrom(0, size1, mProcessor.mAudioBuffer, 0, start2, size2);
        
        mProcessor.mRingBuffer.finishedRead((size1 + size2) / 2);
    }
}
