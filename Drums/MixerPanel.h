#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <array>
#include "ChannelStrip.h"

class MixerPanel : public juce::Component
{
    public:
    MixerPanel() {
        for(auto & channelStrip : channelStrips)
        {
            addAndMakeVisible(channelStrip);
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
            channelStrips[i].setBounds(stripBounds);
        }



    }

    
    std::array<ChannelStrip, 16> channelStrips;
    private:
};