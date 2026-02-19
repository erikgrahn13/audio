#include "DrumsPluginProcessor.h"
#include "DrumsPluginEditor.h"

//==============================================================================
DrumsAudioProcessorEditor::DrumsAudioProcessorEditor (DrumsAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Create channel strips
    // for (int i = 0; i < DrumsAudioProcessor::numChannels; ++i)
    // {
    //     channelStrips[i] = std::make_unique<ChannelStrip>(DrumsAudioProcessor::channelNames[i]);
    //     addAndMakeVisible(channelStrips[i].get());
        
    //     juce::String paramID = juce::String(DrumsAudioProcessor::channelNames[i]) + "Volume";
    //     faderAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
    //         processorRef.mParameters, paramID, channelStrips[i]->fader);
    // }
    
    // Kick pad
    kickPad.setButtonText("KICK");
    kickPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(36, velocity);
    };
    addAndMakeVisible(kickPad);

    snarePad.setButtonText("SNARE");
    snarePad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(38, velocity);
    };
    addAndMakeVisible(snarePad);

    hihatOpenPad.setButtonText("HiHat Open");
    hihatOpenPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(46, velocity);
    };
    addAndMakeVisible(hihatOpenPad);

    hihatClosedPad.setButtonText("HiHat Closed");
    hihatClosedPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(42, velocity);
    };
    addAndMakeVisible(hihatClosedPad);

    tom1Pad.setButtonText("Tom1");
    tom1Pad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(48, velocity);
    };
    addAndMakeVisible(tom1Pad);

    tom2Pad.setButtonText("Tom2");
    tom2Pad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(47, velocity);
    };
    addAndMakeVisible(tom2Pad);

    tom3Pad.setButtonText("Tom3");
    tom3Pad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(45, velocity);
    };
    addAndMakeVisible(tom3Pad);

    tom4Pad.setButtonText("Tom4");
    tom4Pad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(41, velocity);
    };
    addAndMakeVisible(tom4Pad);

    rideLPad.setButtonText("RideL");
    rideLPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(51, velocity);
    };
    addAndMakeVisible(rideLPad);

    rideRPad.setButtonText("RideR");
    rideRPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(59, velocity);
    };
    addAndMakeVisible(rideRPad);

    chinaPad.setButtonText("China");
    chinaPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(52, velocity);
    };
    addAndMakeVisible(chinaPad);
    
    crashLPad.setButtonText("CrashL");
    crashLPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(49, velocity);
    };
    addAndMakeVisible(crashLPad);

    crashRPad.setButtonText("CrashR");
    crashRPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(57, velocity);
    };
    addAndMakeVisible(crashRPad);

    rideLBellPad.setButtonText("RideLBell");
    rideLBellPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(55, velocity);
    };
    addAndMakeVisible(rideLBellPad);

    rideRBellPad.setButtonText("RideRBell");
    rideRBellPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(53, velocity);
    };
    addAndMakeVisible(rideRBellPad);

    snareRestPad.setButtonText("SnareRest");
    snareRestPad.onClick = [this](){
        int velocity = (int)velocitySlider.getValue();
        processorRef.triggerNote(37, velocity);
    };
    addAndMakeVisible(snareRestPad);

    // Velocity slider
    velocitySlider.setRange(0, 127, 1);
    velocitySlider.setValue(100);
    velocitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    velocitySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    addAndMakeVisible(velocitySlider);
    
    velocityLabel.setText("Velocity:", juce::dontSendNotification);
    velocityLabel.attachToComponent(&velocitySlider, true);
    addAndMakeVisible(velocityLabel);

    addAndMakeVisible(mixerPanel);

    setSize (1200, 400);
    startTimerHz(24); // 60fps meter updates
}

DrumsAudioProcessorEditor::~DrumsAudioProcessorEditor()
{
    stopTimer();
}

void DrumsAudioProcessorEditor::timerCallback()
{
    // Get current levels from processor
    // processorRef.getChannelLevels(channelLevels.data(), (int)channelLevels.size());
    
    // Update level meters for all channels
    // for (int i = 0; i < (int)channelStrips.size(); ++i)
    // {
    //     channelStrips[i]->setLevel(channelLevels[i]);
    //     // mixerPanel.channelStrips[i].setLevel(channelLevels[i]);
    // }
    for(int ch = 0; ch < 16; ++ch)
    {
        float level = processorRef.channelLevels[ch].load();
        // meter
        mixerPanel.channelStrips[ch].setLevel(level);
    }

}

//==============================================================================
void DrumsAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
    
    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText ("Kick Drum Mixer", 10, 10, 300, 30, juce::Justification::centredLeft);
}

void DrumsAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Top control area
    auto controlArea = bounds.removeFromTop(80);
    controlArea.removeFromTop(30); // Skip title area
    
    velocitySlider.setBounds(controlArea.removeFromTop(25).reduced(70, 0));
    
    // Place drum pads horizontally
    auto padArea = controlArea.withHeight(30);
    auto left = padArea.getX();
    kickPad.setBounds(left, padArea.getY(), 60, 30);
    snarePad.setBounds(left + 60, padArea.getY(), 60, 30);
    hihatOpenPad.setBounds(left + 120, padArea.getY(), 60, 30);
    hihatClosedPad.setBounds(left + 180, padArea.getY(), 60, 30);
    tom1Pad.setBounds(left + 240, padArea.getY(), 60, 30);
    tom2Pad.setBounds(left + 300, padArea.getY(), 60, 30);
    tom3Pad.setBounds(left + 360, padArea.getY(), 60, 30);
    tom4Pad.setBounds(left + 420, padArea.getY(), 60, 30);
    rideLPad.setBounds(left + 480, padArea.getY(), 60, 30);
    rideRPad.setBounds(left + 540, padArea.getY(), 60, 30);
    chinaPad.setBounds(left + 600, padArea.getY(), 60, 30);
    crashLPad.setBounds(left + 660, padArea.getY(), 60, 30);
    crashRPad.setBounds(left + 720, padArea.getY(), 60, 30);
    rideLBellPad.setBounds(left + 780, padArea.getY(), 60, 30);
    rideRBellPad.setBounds(left + 840, padArea.getY(), 60, 30);
    snareRestPad.setBounds(left + 900, padArea.getY(), 60, 30);
    
    bounds.removeFromTop(10);

    
    
    // Mixer area - 16 channel strips
    auto mixerArea = bounds;
    int channelWidth = mixerArea.getWidth() / (int)mixerPanel.channelStrips.size();
    
    mixerPanel.setBounds(mixerArea);

    // for (int i = 0; i < (int)mixerPanel.channelStrips.size(); ++i)
    // {
    //     auto stripBounds = mixerArea.removeFromLeft(channelWidth).reduced(2);
    //     channelStrips[i]->setBounds(stripBounds);
    // }
}
