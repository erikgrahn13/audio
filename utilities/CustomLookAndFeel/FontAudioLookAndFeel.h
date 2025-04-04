#pragma once

#include <Fonts.h>
#include <JuceHeader.h>

class FontAudioLookAndFeel : public juce::LookAndFeel_V4
{
  public:
    FontAudioLookAndFeel()
        : fontAudio(juce::FontOptions(
              juce::Typeface::createSystemTypefaceFor(CustomFont::fontaudio_ttf, CustomFont::fontaudio_ttfSize)))
    {
    }

    juce::Font &getFont()
    {
        return fontAudio;
    }

    void drawButtonText(juce::Graphics &g, juce::TextButton &button, bool /*isMouseOverButton*/,
                        bool /*isButtonDown*/) override
    {
        auto font = fontAudio;
        g.setFont(font.withHeight(static_cast<float>(button.getHeight())));

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

    // void drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour,
    //                           bool isMouseOverButton, bool isButtonDown) override
    // {
    //     // Do nothing, which means no background will be drawn.
    // }

  private:
    juce::Font fontAudio;
};