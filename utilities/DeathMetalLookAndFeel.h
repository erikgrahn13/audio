#pragma once

#include <JuceHeader.h>
#include <Fonts.h>


class DeathMetalLookAndFeel : public juce::LookAndFeel_V4
{
    public:
    DeathMetalLookAndFeel()
    {
        deathMetalFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::ArtDystopia_ttf, BinaryData::ArtDystopia_ttfSize));
    }

    void drawButtonText (juce::Graphics& g, juce::TextButton& button,
                         bool isMouseOverButton, bool isButtonDown) override
    {
        auto font = deathMetalFont;
        g.setFont (font);
        g.setColour (button.findColour (juce::TextButton::textColourOnId));
        g.drawText (button.getButtonText(), button.getLocalBounds(),
                    juce::Justification::centred, true);

        button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
        button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::black);
    }

    private:
    juce::Font deathMetalFont;
};