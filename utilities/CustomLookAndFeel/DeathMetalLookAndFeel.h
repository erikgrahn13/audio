#pragma once

#include <Fonts.h>
#include <JuceHeader.h>
#include <cmath>
#include <numbers>

class DeathMetalLookAndFeel : public juce::LookAndFeel_V4
{
  public:
    DeathMetalLookAndFeel()
        : deathMetalFont(juce::FontOptions(
              juce::Typeface::createSystemTypefaceFor(CustomFont::ArtDystopia_ttf, CustomFont::ArtDystopia_ttfSize)))
    {
    }
    ~DeathMetalLookAndFeel() override
    {
    }

    juce::Font getLabelFont(juce::Label &label) override
    {
        std::ignore = label;
        return deathMetalFont;
    }

    juce::Font getPopupMenuFont() override
    {
        return deathMetalFont;
    }

    void drawButtonText(juce::Graphics &g, juce::TextButton &button, bool isMouseOverButton, bool isButtonDown) override
    {
        std::ignore = isMouseOverButton;
        std::ignore = isButtonDown;
        auto font = deathMetalFont;
        g.setFont(font);
        g.setColour(button.findColour(juce::TextButton::textColourOnId));
        g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, true);

        button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
        button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::black);
    }

    void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
                          float maxSliderPos, juce::Slider::SliderStyle style, juce::Slider &slider) override
    {
        if (slider.isBar())
        {
            g.setColour(slider.findColour(juce::Slider::trackColourId));
            g.fillRect(slider.isHorizontal() ? juce::Rectangle<float>(static_cast<float>(x), (float)y + 0.5f,
                                                                      sliderPos - (float)x, (float)height - 1.0f)
                                             : juce::Rectangle<float>((float)x + 0.5f, sliderPos, (float)width - 1.0f,
                                                                      (float)y + ((float)height - sliderPos)));

            drawLinearSliderOutline(g, x, y, width, height, style, slider);
        }
        else
        {
            auto isTwoVal = (style == juce::Slider::SliderStyle::TwoValueVertical ||
                             style == juce::Slider::SliderStyle::TwoValueHorizontal);
            auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueVertical ||
                               style == juce::Slider::SliderStyle::ThreeValueHorizontal);

            auto trackWidth = juce::jmin(6.0f, slider.isHorizontal() ? (float)height * 0.25f : (float)width * 0.25f);

            Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
                                    slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

            Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
                                  slider.isHorizontal() ? startPoint.y : (float)y);

            juce::Path backgroundTrack;
            backgroundTrack.startNewSubPath(startPoint);
            backgroundTrack.lineTo(endPoint);
            g.setColour(slider.findColour(juce::Slider::backgroundColourId));
            g.strokePath(backgroundTrack, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});

            juce::Path valueTrack;
            juce::Point<float> minPoint, maxPoint, thumbPoint;

            if (isTwoVal || isThreeVal)
            {
                minPoint = {slider.isHorizontal() ? minSliderPos : (float)width * 0.5f,
                            slider.isHorizontal() ? (float)height * 0.5f : minSliderPos};

                if (isThreeVal)
                    thumbPoint = {slider.isHorizontal() ? sliderPos : (float)width * 0.5f,
                                  slider.isHorizontal() ? (float)height * 0.5f : sliderPos};

                maxPoint = {slider.isHorizontal() ? maxSliderPos : (float)width * 0.5f,
                            slider.isHorizontal() ? (float)height * 0.5f : maxSliderPos};
            }
            else
            {
                auto kx = slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
                auto ky = slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

                minPoint = startPoint;
                maxPoint = {kx, ky};
            }

            auto thumbWidth = getSliderThumbRadius(slider) + 4;

            valueTrack.startNewSubPath(minPoint);
            valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
            g.setColour(slider.findColour(juce::Slider::trackColourId));
            g.strokePath(valueTrack, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});

            if (!isTwoVal)
            {
                g.setColour(juce::Colours::black);
                g.fillEllipse(juce::Rectangle<float>(static_cast<float>(thumbWidth), static_cast<float>(thumbWidth))
                                  .withCentre(isThreeVal ? thumbPoint : maxPoint));

                g.setColour(juce::Colours::white);

                juce::Point<float> point1(maxPoint.getX(), maxPoint.getY());
                point1.setX(point1.getX() + (thumbWidth / 2) * ::sinf(0.f * std::numbers::pi / 180.f));
                point1.setY(point1.getY() + (thumbWidth / 2) * ::cosf(0.f * std::numbers::pi / 180.f));

                juce::Point<float> point2(maxPoint.getX(), maxPoint.getY());
                point2.setX(point2.getX() +
                            (thumbWidth / 2) * ::sinf(144.f * static_cast<float>(std::numbers::pi) / 180.f));
                point2.setY(point2.getY() +
                            (thumbWidth / 2) * ::cosf(144.f * static_cast<float>(std::numbers::pi) / 180.f));

                juce::Point<float> point3(maxPoint.getX(), maxPoint.getY());
                point3.setX(point3.getX() +
                            (thumbWidth / 2) * ::sinf(288.f * static_cast<float>(std::numbers::pi) / 180.f));
                point3.setY(point3.getY() +
                            (thumbWidth / 2) * ::cosf(288.f * static_cast<float>(std::numbers::pi) / 180.f));

                juce::Point<float> point4(maxPoint.getX(), maxPoint.getY());
                point4.setX(point4.getX() +
                            (thumbWidth / 2) * ::sinf(72.f * static_cast<float>(std::numbers::pi) / 180.f));
                point4.setY(point4.getY() +
                            (thumbWidth / 2) * ::cosf(72.f * static_cast<float>(std::numbers::pi) / 180.f));

                juce::Point<float> point5(maxPoint.getX(), maxPoint.getY());
                point5.setX(point5.getX() +
                            (thumbWidth / 2) * ::sinf(216.f * static_cast<float>(std::numbers::pi) / 180.f));
                point5.setY(point5.getY() +
                            (thumbWidth / 2) * ::cosf(216.f * static_cast<float>(std::numbers::pi) / 180.f));

                juce::Path pentagramPath;
                pentagramPath.startNewSubPath(point1);
                pentagramPath.lineTo(point2);
                pentagramPath.lineTo(point3);
                pentagramPath.lineTo(point4);
                pentagramPath.lineTo(point5);
                pentagramPath.closeSubPath();
                g.strokePath(pentagramPath, juce::PathStrokeType(1));

                g.drawEllipse(juce::Rectangle<float>(static_cast<float>(thumbWidth), static_cast<float>(thumbWidth))
                                  .withCentre(isThreeVal ? thumbPoint : maxPoint)
                                  .reduced(2, 2),
                              1.);
            }

            if (isTwoVal || isThreeVal)
            {
                auto sr = juce::jmin(trackWidth, (slider.isHorizontal() ? (float)height : (float)width) * 0.4f);
                auto pointerColour = slider.findColour(juce::Slider::thumbColourId);

                if (slider.isHorizontal())
                {
                    drawPointer(g, minSliderPos - sr,
                                juce::jmax(0.0f, (float)y + (float)height * 0.5f - trackWidth * 2.0f),
                                trackWidth * 2.0f, pointerColour, 2);

                    drawPointer(g, maxSliderPos - trackWidth,
                                juce::jmin((float)(y + height) - trackWidth * 2.0f, (float)y + (float)height * 0.5f),
                                trackWidth * 2.0f, pointerColour, 4);
                }
                else
                {
                    drawPointer(g, juce::jmax(0.0f, (float)x + (float)width * 0.5f - trackWidth * 2.0f),
                                minSliderPos - trackWidth, trackWidth * 2.0f, pointerColour, 1);

                    drawPointer(g, juce::jmin((float)(x + width) - trackWidth * 2.0f, (float)x + (float)width * 0.5f),
                                maxSliderPos - sr, trackWidth * 2.0f, pointerColour, 3);
                }
            }

            if (slider.isBar())
                drawLinearSliderOutline(g, x, y, width, height, style, slider);
        }
    }

    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPosProportional,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override
    {
        std::ignore = slider;
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        // auto lineW = jmin (5.0f, radius * 0.5f);
        auto lineW = radius * 0.135f;

        juce::Rectangle<float> bounds(rx, ry, rw, rw);

        // Draw the black ellipse background
        g.setColour(juce::Colours::black);
        g.fillEllipse(bounds);

        // auto arcThickness = 4.0;
        float adjustedRadius = radius - (lineW / 2.0f);
        // Draw the grey arc
        juce::Path backgroundArc;
        backgroundArc.addCentredArc(centreX, centreY, adjustedRadius, adjustedRadius, 0.0f, rotaryStartAngle,
                                    rotaryEndAngle, true);

        g.setColour(juce::Colours::grey.darker(1.0));
        g.strokePath(backgroundArc,
                     juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

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

            offscreenGraphics.strokePath(
                arcPath, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            // offscreenGraphics.strokePath(arcPath, juce::PathStrokeType(lineW, juce::PathStrokeType::curved,
            // juce::PathStrokeType::rounded));
        }

        // Create a new image to unbind the offscreen image
        juce::Image unboundImage = offscreenImage.createCopy();

        // Apply the glow effect to the unbound image
        juce::Image glowImage(juce::Image::ARGB, width, height, true);
        {
            juce::Graphics glowGraphics(glowImage);
            juce::GlowEffect glowEffect;
            glowEffect.setGlowProperties(5.0f, juce::Colours::yellow);
            glowEffect.applyEffect(unboundImage, glowGraphics, 1.0f, 1.0f);
        }

        // Draw the image with the glow effect
        g.drawImageAt(glowImage, x, y);

        g.setColour(juce::Colours::white);

        juce::Point<float> point1(centreX, centreY);
        point1.setX(point1.getX() + (radius - lineW * 1.5f) * ::sinf(0.f * std::numbers::pi / 180.f));
        point1.setY(point1.getY() + (radius - lineW * 1.5f) * ::cosf(0.f * std::numbers::pi / 180.f));

        juce::Point<float> point2(centreX, centreY);
        point2.setX(point2.getX() +
                    (radius - lineW * 1.5f) * ::sinf(144.f * static_cast<float>(std::numbers::pi) / 180.f));
        point2.setY(point2.getY() +
                    (radius - lineW * 1.5f) * ::cosf(144.f * static_cast<float>(std::numbers::pi) / 180.f));

        juce::Point<float> point3(centreX, centreY);
        point3.setX(point3.getX() +
                    (radius - lineW * 1.5f) * ::sinf(288.f * static_cast<float>(std::numbers::pi) / 180.f));
        point3.setY(point3.getY() +
                    (radius - lineW * 1.5f) * ::cosf(288.f * static_cast<float>(std::numbers::pi) / 180.f));

        juce::Point<float> point4(centreX, centreY);
        point4.setX(point4.getX() +
                    (radius - lineW * 1.5f) * ::sinf(72.f * static_cast<float>(std::numbers::pi) / 180.f));
        point4.setY(point4.getY() +
                    (radius - lineW * 1.5f) * ::cosf(72.f * static_cast<float>(std::numbers::pi) / 180.f));

        juce::Point<float> point5(centreX, centreY);
        point5.setX(point5.getX() +
                    (radius - lineW * 1.5f) * ::sinf(216.f * static_cast<float>(std::numbers::pi) / 180.f));
        point5.setY(point5.getY() +
                    (radius - lineW * 1.5f) * ::cosf(216.f * static_cast<float>(std::numbers::pi) / 180.f));

        juce::Path pentagramPath;
        pentagramPath.startNewSubPath(point1);
        pentagramPath.lineTo(point2);
        pentagramPath.lineTo(point3);
        pentagramPath.lineTo(point4);
        pentagramPath.lineTo(point5);
        pentagramPath.closeSubPath();

        // Apply rotation around the center point
        juce::AffineTransform transform =
            juce::AffineTransform::translation(-centreX, -centreY).rotated(angle).translated(centreX, centreY);
        pentagramPath.applyTransform(transform);

        g.strokePath(pentagramPath, juce::PathStrokeType(2));

        // Draw inner circle
        g.drawEllipse(bounds.reduced(lineW * 2.f), 2.f);

        // Draw upside-down cross
        juce::Path crossPath;

        // Vertical part of the cross
        crossPath.addRectangle(centreX - lineW / 2, centreY + radius / 6, lineW, -radius);
        // crossPath.addRectangle(-crossWidth / 2.0f, -crossHeight / 2.0f, crossWidth, crossHeight);

        // Horizontal part of the cross (near the bottom)
        crossPath.addRectangle(centreX - (radius / 3 / 2), centreY - radius / 10, radius / 3, lineW);
        // crossPath.addRectangle(-crossWidth / 2.0f, crossHeight / 4.0f, crossHeight / 2.0f, crossWidth);

        // Apply the same transform
        crossPath.applyTransform(transform);

        g.setColour(juce::Colours::white);
        g.fillPath(crossPath);
    }

    void drawGroupComponentOutline(juce::Graphics &g, int width, int height, const juce::String &text,
                                   const juce::Justification &position, juce::GroupComponent &group) override
    {
        const float textH = 15.0f;
        const float indent = 3.0f;
        const float textEdgeGap = 4.0f;
        auto cs = 5.0f;

        juce::Path p;
        auto x = indent;
        auto y = deathMetalFont.getAscent() - 3.0f;
        auto w = juce::jmax(0.0f, (float)width - x * 2.0f);
        auto h = juce::jmax(0.0f, (float)height - y - indent);
        cs = juce::jmin(cs, w * 0.5f, h * 0.5f);
        auto cs2 = 2.0f * cs;

        auto textW = text.isEmpty() ? 0 : juce::jlimit(0.0f, juce::jmax(0.0f, w - cs2 - textEdgeGap * 2), [&]() {
            // Create a TextLayout and calculate the width
            juce::TextLayout layout;
            juce::AttributedString attributedText;
            attributedText.append(text, deathMetalFont, juce::Colours::black); // Adjust the color as needed

            layout.createLayout(attributedText, w - cs2 - textEdgeGap * 2);
            return layout.getWidth() + textEdgeGap * 2.0f;
        }());
        auto textX = cs + textEdgeGap;

        if (position.testFlags(juce::Justification::horizontallyCentred))
            textX = cs + (w - cs2 - textW) * 0.5f;
        else if (position.testFlags(juce::Justification::right))
            textX = w - cs - textW - textEdgeGap;

        p.startNewSubPath(x + textX + textW, y);
        p.lineTo(x + w - cs, y);

        p.addArc(x + w - cs2, y, cs2, cs2, 0, juce::MathConstants<float>::halfPi);
        p.lineTo(x + w, y + h - cs);

        p.addArc(x + w - cs2, y + h - cs2, cs2, cs2, juce::MathConstants<float>::halfPi,
                 juce::MathConstants<float>::pi);
        p.lineTo(x + cs, y + h);

        p.addArc(x, y + h - cs2, cs2, cs2, juce::MathConstants<float>::pi, juce::MathConstants<float>::pi * 1.5f);
        p.lineTo(x, y + cs);

        p.addArc(x, y, cs2, cs2, juce::MathConstants<float>::pi * 1.5f, juce::MathConstants<float>::twoPi);
        p.lineTo(x + textX, y);

        auto alpha = group.isEnabled() ? 1.0f : 0.5f;

        g.setColour(group.findColour(juce::GroupComponent::outlineColourId).withMultipliedAlpha(alpha));

        g.strokePath(p, juce::PathStrokeType(2.0f));

        g.setColour(group.findColour(juce::GroupComponent::textColourId).withMultipliedAlpha(alpha));
        g.setFont(deathMetalFont);
        g.drawText(text, juce::roundToInt(x + textX), 0, juce::roundToInt(textW), juce::roundToInt(textH),
                   juce::Justification::centred, true);
    }

    void drawPopupMenuBackground(juce::Graphics &g, int width, int height) override
    {
        std::ignore = height;
        std::ignore = width;
        g.fillAll(juce::Colours::black); // Custom background color for the popup menu
    }

  private:
    juce::Font deathMetalFont;
    juce::ColourGradient mGradient;
};