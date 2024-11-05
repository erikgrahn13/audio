#pragma once

#include <Fonts.h>
#include <JuceHeader.h>

class FontWebLookAndFeel : public juce::LookAndFeel_V4
{
  public:
    FontWebLookAndFeel()
        : fontWeb(juce::FontOptions(juce::Typeface::createSystemTypefaceFor(CustomFont::forkawesomewebfont_ttf,
                                                                            CustomFont::forkawesomewebfont_ttfSize)))
    {
    }

    juce::Font &getFont()
    {
        return fontWeb;
    }

    void drawButtonText(juce::Graphics &g, juce::TextButton &button, bool /*isMouseOverButton*/,
                        bool /*isButtonDown*/) override
    {
        auto font = fontWeb;
        g.setFont(font.withHeight(static_cast<float>(button.getHeight() * 0.6)));

        juce::Colour textColour;
        if (button.getClickingTogglesState() && button.getToggleState())
            textColour = button.findColour(juce::TextButton::textColourOnId);
        else
            textColour = button.findColour(juce::TextButton::textColourOffId);

        g.setColour(textColour);

        // juce::Colour backgroundColor;
        // if (button.getClickingTogglesState() && button.getToggleState())
        // backgroundColor = button.findColour(juce::TextButton::buttonOnColourId);
        // else
        // backgroundColor = button.findColour(juce::TextButton::buttonColourId);

        button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);

        g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, true);
    }

  private:
    juce::Font fontWeb;
};