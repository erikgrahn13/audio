#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <array>
#include "ChannelStrip.h"
#include "constants.h"

class MixerPanel : public juce::Component
{
    public:
    MixerPanel(juce::AudioProcessorValueTreeState &parameters) {
        // for(auto & channelStrip : channelStrips)
        // {
        //     addAndMakeVisible(channelStrip);
        // }
        
        for(int i = 0; i < numChannels; ++i)
        {
            juce::String ParamID = juce::String(channelNames[i]) + "Volume";

            channelStrips.push_back(std::make_unique<ChannelStrip>(parameters, ParamID));
            addAndMakeVisible(channelStrips.at(i).get());
        }

        // startTimerHz(24);
    }

    void paint(juce::Graphics& g) override
    {
            g.fillAll (juce::Colours::black);

    }

    // void timerCallback() override
    // {
    //     for(auto & levelMeter : levelMeters)
    //     {
    //         levelMeter.setLevel(level);
    //         levelMeter.repaint();
    //     }
    // }

    void resized() override
    {
        auto bounds = getLocalBounds();
        int channelWidth = bounds.getWidth() / (int)channelStrips.size();

        for (int i = 0; i < (int)channelStrips.size(); ++i)
        {
            auto stripBounds = bounds.removeFromLeft(channelWidth).reduced(2);
            channelStrips[i]->setBounds(stripBounds);
        }



    }

    
    // std::array<ChannelStrip, 16> channelStrips;
    std::vector<std::unique_ptr<ChannelStrip>> channelStrips;
    private:
};