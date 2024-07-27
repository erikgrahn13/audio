#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p,
                                                                 juce::AudioProcessorValueTreeState &parameters)
    : AudioProcessorEditor(&p), processorRef(p), mParameters(parameters), mEQView(p, parameters), mAnalyzerCurve(p),
      mHPFSliderGroup(p, parameters, "hpf_freq", "", ""),
      mLowShelfSliderGroup(p, parameters, "LowShelfFreq", "LowShelfGain", ""),
      mLowMidSliderGroup(p, parameters, "LowMidFreq", "LowMidGain", "LowMidQ"),
      mHighMidSliderGroup(p, parameters, "HighMidFreq", "HighMidGain", "HighMidQ"),
      mHighShelfSliderGroup(p, parameters, "HighShelfFreq", "HighShelfGain", ""),
      mLPFSliderGroup(p, parameters, "lpf_freq", "", "")
{
    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(mAnalyzerCurve);
    addAndMakeVisible(mEQView);
    setSize(900, 500);

    for (int i = 0; i < mParameters.state.getNumChildren(); ++i)
    {
        auto paramID = mParameters.state.getChild(i).getProperty("id").toString();
        mParameters.addParameterListener(paramID, this);
    }

    mHPFSliderGroup.setText("1");
    mHPFSliderGroup.setSymbolType(juce::CharPointer_UTF8(""));

    mLowShelfSliderGroup.setText("2");
    mLowShelfSliderGroup.setSymbolType(juce::CharPointer_UTF8(""));

    mLowMidSliderGroup.setText("3");
    mLowMidSliderGroup.setSymbolType(juce::CharPointer_UTF8(""));

    mHighMidSliderGroup.setText("4");
    mHighMidSliderGroup.setSymbolType(juce::CharPointer_UTF8(""));

    mHighShelfSliderGroup.setText("5");
    mHighShelfSliderGroup.setSymbolType(juce::CharPointer_UTF8(""));

    mLPFSliderGroup.setText("6");
    mLPFSliderGroup.setSymbolType(juce::CharPointer_UTF8(""));

    addAndMakeVisible(mHPFSliderGroup);
    addAndMakeVisible(mLowShelfSliderGroup);
    addAndMakeVisible(mLowMidSliderGroup);
    addAndMakeVisible(mHighMidSliderGroup);
    addAndMakeVisible(mHighShelfSliderGroup);
    addAndMakeVisible(mLPFSliderGroup);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    for (int i = 0; i < mParameters.state.getNumChildren(); ++i)
    {
        auto paramID = mParameters.state.getChild(i).getProperty("id").toString();
        mParameters.removeParameterListener(paramID, this);
    }
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.setColour(Colours::black);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    auto graphArea = bounds.removeFromTop(bounds.getHeight() / 2);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mAnalyzerCurve.setBounds(graphArea.reduced(20));
    mEQView.setBounds(graphArea.reduced(20));
    auto sliderGroupArea = bounds.getWidth() / 6;

    mHPFSliderGroup.setBounds(bounds.removeFromLeft(sliderGroupArea));
    mLowShelfSliderGroup.setBounds(bounds.removeFromLeft(sliderGroupArea));
    mLowMidSliderGroup.setBounds(bounds.removeFromLeft(sliderGroupArea));
    mHighMidSliderGroup.setBounds(bounds.removeFromLeft(sliderGroupArea));
    mHighShelfSliderGroup.setBounds(bounds.removeFromLeft(sliderGroupArea));
    mLPFSliderGroup.setBounds(bounds.removeFromLeft(sliderGroupArea));
}

void AudioPluginAudioProcessorEditor::parameterChanged(const juce::String &parameterID, float newValue)
{
    auto &handles = mEQView.getHandles();
    bool needsUpdate{false};

    for (auto &handle : handles)
    {
        if (handle->mFreqParameter->getParameterID() == parameterID)
        {
            handle->biquad.setFrequency(newValue);
            needsUpdate = true;
        }

        if (handle->mGainParameter && handle->mGainParameter->getParameterID() == parameterID)
        {
            handle->biquad.setGain(newValue);
            needsUpdate = true;
        }

        if (handle->mQParameter && handle->mQParameter->getParameterID() == parameterID)
        {
            handle->biquad.setQ(newValue);
            needsUpdate = true;
        }
    }

    if (needsUpdate)
    {
        juce::MessageManager::callAsync([this]() { mEQView.updateFrequencyResponse(); });
    }
}
