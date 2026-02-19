#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class ChannelStrip : public juce::Component
{
public:
	ChannelStrip()
	{
		volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
	    volumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 0, 0);

		addAndMakeVisible(volumeSlider);
		addAndMakeVisible(levelMeter);
	}

	class LevelMeter : public juce::Component
	{
		public:
			void paint(juce::Graphics& g) override
			{
				auto bounds = getLocalBounds().toFloat();

				g.setColour(juce::Colours::white.withBrightness(0.4f));
				g.fillRoundedRectangle(bounds, 5.f);

				g.setColour(juce::Colours::white);
				// map level from -60.f / +6.f  to  0 / width
				const auto scaledY = 
					juce::jmap(level, -60.f, +6.f, 0.f, static_cast<float>(getHeight()));
				g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 5.f);
			}

			void setLevel(const float value) { level = value; }
		private:
			float level = -60.f;
	};

	void resized() override
	{
		auto bounds = getLocalBounds();

		volumeSlider.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
		levelMeter.setBounds(bounds);
	}

	void setLevel(float level)
	{
		levelMeter.setLevel(level);
	}


	private:
	LevelMeter levelMeter;
	juce::Slider volumeSlider;
};