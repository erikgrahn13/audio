#pragma once

#include <JuceHeader.h>
#include <Fonts.h>
#include <numbers>


class DeathMetalLookAndFeel : public juce::LookAndFeel_V4
{
    public:
    DeathMetalLookAndFeel()
    {
        deathMetalFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::ArtDystopia_ttf, BinaryData::ArtDystopia_ttfSize));
        
    }
    ~DeathMetalLookAndFeel()
    {

    }

    juce::Font getLabelFont(juce::Label& label) override
    {
        return deathMetalFont;
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

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto radius = (float) juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float) x + (float) width * 0.5f;
        auto centreY = (float) y + (float) height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        // auto lineW = jmin (5.0f, radius * 0.5f);
        auto lineW = radius * 0.135;


        juce::Rectangle<float> bounds(rx, ry, rw, rw);

        // Draw the black ellipse background
        g.setColour(juce::Colours::black);
        g.fillEllipse(bounds);


        // auto arcThickness = 4.0;
        float adjustedRadius = radius - (lineW / 2.0f);
        // Draw the grey arc 
        Path backgroundArc;
        backgroundArc.addCentredArc (centreX,
                                 centreY,
                                 adjustedRadius,
                                 adjustedRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);

        g.setColour (juce::Colours::grey.darker(1.0));
        g.strokePath (backgroundArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));



        // Draw the white arc indicating the value
        juce::Path arcPath;
        arcPath.addCentredArc(centreX, centreY, adjustedRadius, adjustedRadius, 0.0f, rotaryStartAngle, angle, true);
        

        // Create an offscreen image to draw the arc
        juce::Image offscreenImage(juce::Image::ARGB, width, height, true);
        {
            juce::Graphics offscreenGraphics(offscreenImage);

            // Fill with transparency to avoid any artifacts
            offscreenGraphics.fillAll(juce::Colours::transparentBlack);

            // Draw the arc path to the offscreen image
            offscreenGraphics.setColour(juce::Colours::white);

            offscreenGraphics.strokePath(arcPath, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            // offscreenGraphics.strokePath(arcPath, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // Create a new image to unbind the offscreen image
        juce::Image unboundImage = offscreenImage.createCopy();

        // Apply the glow effect to the unbound image
        juce::Image glowImage(juce::Image::ARGB, width, height, true);
        {
            juce::Graphics glowGraphics(glowImage);
            juce::GlowEffect glowEffect;
            glowEffect.setGlowProperties(10.0f, juce::Colours::yellow);
            glowEffect.applyEffect(unboundImage, glowGraphics, 1.0f, 1.0f);
        }

        // Draw the image with the glow effect
        g.drawImageAt(glowImage, x, y);

    g.setColour(juce::Colours::white);

    juce::Point<float> point1(centreX, centreY);
    point1.setX(point1.getX() + (radius - lineW*1.5) * std::sin(0. * std::numbers::pi / 180.));
    point1.setY(point1.getY() + (radius - lineW*1.5) * std::cos(0. * std::numbers::pi / 180.));

    juce::Point<float> point2(centreX, centreY);
    point2.setX(point2.getX() + (radius - lineW*1.5) * std::sin(144. * std::numbers::pi / 180.));
    point2.setY(point2.getY() + (radius - lineW*1.5) * std::cos(144. * std::numbers::pi / 180.));

    juce::Point<float> point3(centreX, centreY);
    point3.setX(point3.getX() + (radius - lineW*1.5) * std::sin(288. * std::numbers::pi / 180.));
    point3.setY(point3.getY() + (radius - lineW*1.5) * std::cos(288. * std::numbers::pi / 180.));

    juce::Point<float> point4(centreX, centreY);
    point4.setX(point4.getX() + (radius - lineW*1.5) * std::sin(72. * std::numbers::pi / 180.));
    point4.setY(point4.getY() + (radius - lineW*1.5) * std::cos(72. * std::numbers::pi / 180.));

    juce::Point<float> point5(centreX, centreY);
    point5.setX(point5.getX() + (radius - lineW*1.5) * std::sin(216. * std::numbers::pi / 180.));
    point5.setY(point5.getY() + (radius - lineW*1.5) * std::cos(216. * std::numbers::pi / 180.));

    juce::Path pentagramPath;
    pentagramPath.startNewSubPath(point1);
    pentagramPath.lineTo(point2);
    pentagramPath.lineTo(point3);
    pentagramPath.lineTo(point4);
    pentagramPath.lineTo(point5);
    pentagramPath.closeSubPath();
    
    // Apply rotation around the center point
    juce::AffineTransform transform = juce::AffineTransform::translation(-centreX, -centreY)
                                      .rotated(angle)
                                      .translated(centreX, centreY);
    pentagramPath.applyTransform(transform);

    g.strokePath(pentagramPath, juce::PathStrokeType(2));

    // Draw inner circle
    g.drawEllipse(bounds.reduced(lineW*2.0), 2.0);

        // Draw upside-down cross
        juce::Path crossPath;

        // Vertical part of the cross
        crossPath.addRectangle(centreX - lineW / 2, centreY + radius / 6, lineW, -radius);
        // crossPath.addRectangle(-crossWidth / 2.0f, -crossHeight / 2.0f, crossWidth, crossHeight);

        // Horizontal part of the cross (near the bottom)
        crossPath.addRectangle(centreX - (radius / 3 / 2),centreY - radius / 10, radius / 3, lineW);
        // crossPath.addRectangle(-crossWidth / 2.0f, crossHeight / 4.0f, crossHeight / 2.0f, crossWidth);

        // Apply the same transform
        crossPath.applyTransform(transform);

        g.setColour(juce::Colours::white);
        g.fillPath(crossPath);
    }

    void drawGroupComponentOutline(juce::Graphics& g, int width, int height,
                                   const juce::String& text, const juce::Justification& position,
                                   juce::GroupComponent& group) override
    {
        const float textH = 15.0f;
        const float indent = 3.0f;
        const float textEdgeGap = 4.0f;
        auto cs = 5.0f;


        Path p;
        auto x = indent;
        auto y = deathMetalFont.getAscent() - 3.0f;
        auto w = jmax (0.0f, (float) width - x * 2.0f);
        auto h = jmax (0.0f, (float) height - y  - indent);
        cs = jmin (cs, w * 0.5f, h * 0.5f);
        auto cs2 = 2.0f * cs;

        auto textW = text.isEmpty() ? 0
                                    : jlimit (0.0f,
                                            jmax (0.0f, w - cs2 - textEdgeGap * 2),
                                            (float) deathMetalFont.getStringWidth (text) + textEdgeGap * 2.0f);
        auto textX = cs + textEdgeGap;

        if (position.testFlags (Justification::horizontallyCentred))
            textX = cs + (w - cs2 - textW) * 0.5f;
        else if (position.testFlags (Justification::right))
            textX = w - cs - textW - textEdgeGap;

        p.startNewSubPath (x + textX + textW, y);
        p.lineTo (x + w - cs, y);

        p.addArc (x + w - cs2, y, cs2, cs2, 0, MathConstants<float>::halfPi);
        p.lineTo (x + w, y + h - cs);

        p.addArc (x + w - cs2, y + h - cs2, cs2, cs2, MathConstants<float>::halfPi, MathConstants<float>::pi);
        p.lineTo (x + cs, y + h);

        p.addArc (x, y + h - cs2, cs2, cs2, MathConstants<float>::pi, MathConstants<float>::pi * 1.5f);
        p.lineTo (x, y + cs);

        p.addArc (x, y, cs2, cs2, MathConstants<float>::pi * 1.5f, MathConstants<float>::twoPi);
        p.lineTo (x + textX, y);

        auto alpha = group.isEnabled() ? 1.0f : 0.5f;

        g.setColour (group.findColour (GroupComponent::outlineColourId)
                        .withMultipliedAlpha (alpha));

        g.strokePath (p, PathStrokeType (2.0f));

        g.setColour (group.findColour (GroupComponent::textColourId)
                        .withMultipliedAlpha (alpha));
        g.setFont (deathMetalFont);
        g.drawText (text,
                    roundToInt (x + textX), 0,
                    roundToInt (textW),
                    roundToInt (textH),
                    Justification::centred, true);

    }

    // void drawGroupComponentOutline(juce::Graphics& g, int width, int height,
    //                                const juce::String& text, const juce::Justification& position,
    //                                juce::GroupComponent& group) override
    // {
    //     const int textHeight = 20; // Height of the text area

    //     g.setColour(group.findColour(juce::GroupComponent::outlineColourId));
    //     g.drawRect(0, textHeight / 2, width, height - textHeight / 2);

    //     g.setColour(group.findColour(juce::GroupComponent::textColourId));
    //     g.setFont(deathMetalFont); // Set the custom font

    //     juce::Rectangle<int> textArea(10, 0, width - 20, textHeight);
    //     g.drawFittedText(text, textArea, position, 1);
    // }


    // juce::Font getLabelFont(juce::Label& label) override
    // {
    //     return deathMetalFont;
    // }

    private:
    juce::Font deathMetalFont;
    juce::ColourGradient mGradient;
};