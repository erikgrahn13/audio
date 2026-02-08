#pragma once

#include "DrumsPluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                               private juce::Timer
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    // Channel strip component (fader + meter + label)
    struct ChannelStrip : public juce::Component
    {
        ChannelStrip(const juce::String& channelName)
        {
            label.setText(channelName, juce::dontSendNotification);
            label.setJustificationType(juce::Justification::centred);
            addAndMakeVisible(label);
            
            fader.setSliderStyle(juce::Slider::LinearVertical);
            fader.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            addAndMakeVisible(fader);
            
            addAndMakeVisible(meter);
        }
        
        void resized() override
        {
            auto bounds = getLocalBounds();
            label.setBounds(bounds.removeFromBottom(20));
            bounds.removeFromBottom(2);
            
            auto meterWidth = 8;
            meter.setBounds(bounds.removeFromLeft(meterWidth));
            bounds.removeFromLeft(2);
            fader.setBounds(bounds);
        }
        
        void setLevel(float level)
        {
            currentLevel = level;
            meter.repaint();
        }
        
        void paint(juce::Graphics& g) override
        {
            // Meter background
            auto meterBounds = meter.getBounds();
            g.setColour(juce::Colours::black);
            g.fillRect(meterBounds);
            
            // Level indicator
            if (currentLevel > 0.0f)
            {
                float dbLevel = juce::Decibels::gainToDecibels(currentLevel);
                float normalised = juce::jmap(dbLevel, -60.0f, 0.0f, 0.0f, 1.0f);
                normalised = juce::jlimit(0.0f, 1.0f, normalised);
                
                int levelHeight = (int)(meterBounds.getHeight() * normalised);
                
                g.setGradientFill(juce::ColourGradient(
                    juce::Colours::red, 0.0f, (float)meterBounds.getY(),
                    juce::Colours::green, 0.0f, (float)meterBounds.getBottom(),
                    false));
                g.fillRect(meterBounds.getX(), 
                          meterBounds.getBottom() - levelHeight,
                          meterBounds.getWidth(), 
                          levelHeight);
            }
        }
        
        juce::Slider fader;
        juce::Label label;
        juce::Component meter;
        float currentLevel = 0.0f;
    };
    
    AudioPluginAudioProcessor& processorRef;
    
    juce::TextButton kickPad;
    juce::Slider velocitySlider;
    juce::Label velocityLabel;
    
    // 16 channel strips
    std::array<std::unique_ptr<ChannelStrip>, 16> channelStrips;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 16> faderAttachments;
    std::array<float, 16> channelLevels{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
