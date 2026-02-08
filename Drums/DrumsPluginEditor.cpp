#include "DrumsPluginProcessor.h"
#include "DrumsPluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Create channel strips
    for (int i = 0; i < AudioPluginAudioProcessor::numChannels; ++i)
    {
        channelStrips[i] = std::make_unique<ChannelStrip>(AudioPluginAudioProcessor::channelNames[i]);
        addAndMakeVisible(channelStrips[i].get());
        
        juce::String paramID = juce::String(AudioPluginAudioProcessor::channelNames[i]) + "Volume";
        faderAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.mParameters, paramID, channelStrips[i]->fader);
    }
    
    // Kick pad
    kickPad.setButtonText("KICK");
    kickPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerKickNote(velocity);
    };
    addAndMakeVisible(kickPad);
    
    // Velocity slider
    velocitySlider.setRange(0, 127, 1);
    velocitySlider.setValue(100);
    velocitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    velocitySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    addAndMakeVisible(velocitySlider);
    
    velocityLabel.setText("Velocity:", juce::dontSendNotification);
    velocityLabel.attachToComponent(&velocitySlider, true);
    addAndMakeVisible(velocityLabel);

    setSize (1200, 400);
    startTimerHz(60); // 60fps meter updates
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    stopTimer();
}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    // Get current levels from processor
    processorRef.getChannelLevels(channelLevels.data(), (int)channelLevels.size());
    
    // Update level meters for all channels
    for (int i = 0; i < (int)channelStrips.size(); ++i)
    {
        channelStrips[i]->setLevel(channelLevels[i]);
    }
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
    
    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText ("Kick Drum Mixer", 10, 10, 300, 30, juce::Justification::centredLeft);
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Top control area
    auto controlArea = bounds.removeFromTop(80);
    controlArea.removeFromTop(30); // Skip title area
    
    velocitySlider.setBounds(controlArea.removeFromTop(25).reduced(70, 0));
    kickPad.setBounds(controlArea.withSizeKeepingCentre(100, 30));
    
    bounds.removeFromTop(10);
    
    // Mixer area - 16 channel strips
    auto mixerArea = bounds;
    int channelWidth = mixerArea.getWidth() / (int)channelStrips.size();
    
    for (int i = 0; i < (int)channelStrips.size(); ++i)
    {
        auto stripBounds = mixerArea.removeFromLeft(channelWidth).reduced(2);
        channelStrips[i]->setBounds(stripBounds);
    }
}
