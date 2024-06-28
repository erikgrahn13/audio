#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p, juce::AudioProcessorValueTreeState& parameters)
    : AudioProcessorEditor(&p), 
      processorRef(p), 
      mParameters(parameters), 
      mEQView(p, parameters),
      mAnalyzerCurve(p)
{
    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(mAnalyzerCurve);
    addAndMakeVisible(mEQView);
    setSize(400, 300);

    for(int i = 0; i < mParameters.state.getNumChildren(); ++i)
    {
        auto paramID = mParameters.state.getChild(i).getProperty("id").toString();
        mParameters.addParameterListener(paramID, this);
    }
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    for(int i = 0; i < mParameters.state.getNumChildren(); ++i)
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
    const auto area = getLocalBounds().reduced(20);
    auto bounds = area;
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mAnalyzerCurve.setBounds(bounds.reduced(20));
    mEQView.setBounds(bounds.reduced(20));
}

void AudioPluginAudioProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
    auto &handles = mEQView.getHandles();
    bool needsUpdate{false};

    for(auto& handle : handles)
    {
        if(handle->mFreqParameter->getParameterID() == parameterID)
        {
            handle->biquad.setFrequency(newValue);
            needsUpdate = true;
        }

        if(handle->mGainParameter && handle->mGainParameter->getParameterID() == parameterID)
        {
            handle->biquad.setGain(newValue);
            needsUpdate = true;
        }

        if(handle->mQParameter && handle->mQParameter->getParameterID() == parameterID)
        {
            handle->biquad.setQ(newValue);
            needsUpdate = true;
        }
    }

    if(needsUpdate)
    {
        juce::MessageManager::callAsync([this]() {
                mEQView.updateFrequencyResponse();
            });
    }
}

