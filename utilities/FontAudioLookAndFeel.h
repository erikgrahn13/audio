#pragma once

#include <Fonts.h>
#include <JuceHeader.h>

class FontAudioLookAndFeel : public juce::LookAndFeel_V4
{
  public:
    FontAudioLookAndFeel()
        : fontAudio(juce::FontOptions(
              juce::Typeface::createSystemTypefaceFor(BinaryData::fontaudio_ttf, BinaryData::fontaudio_ttfSize)))
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
        g.setFont(font.withHeight(button.getHeight() * 0.6f));

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
        button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkgrey.darker());

        g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, true);
    }

  private:
    juce::Font fontAudio;
};