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

    void drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour,
                              bool isMouseOverButton, bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        std::ignore = isMouseOverButton;
        // Set custom colors or remove highlight effect
        juce::Colour fillColour = backgroundColour;

        if (isButtonDown)
            fillColour = backgroundColour; // Keep original background color (no highlight)
        // else if (isMouseOverButton)
        //     fillColour = backgroundColour.brighter(); // Optional: a subtle hover effect

        g.setColour(fillColour);
        g.fillRect(bounds);

        // Optional: Draw border
        // g.setColour(button.findColour(juce::ComboBox::outlineColourId));
        // g.drawRect(bounds, 1.0f);
    }

  private:
    juce::Font fontWeb;
};