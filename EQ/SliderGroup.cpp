#include "SliderGroup.h"

SliderGroup::SliderGroup(AudioPluginAudioProcessor &processor, std::string_view frequencyParameterID,
                         std::string_view gainParameterID, std::string_view QParameterID)
    : mProcessor(processor)
{
    frame.setLookAndFeel(&deathMetalLookAndFeel);
    frame.setTextLabelPosition(juce::Justification::centred);
    addAndMakeVisible(frame);

    fontAudioLookAndFeel.getFont().setHeight(48.0f);
    filterSymbolLabel.setFont(fontAudioLookAndFeel.getFont());
    filterSymbolLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filterSymbolLabel);

    mFrequencySlider.setBufferedToImage(true);
    mGainSlider.setBufferedToImage(true);
    mQSlider.setBufferedToImage(true);

    if (!frequencyParameterID.empty())
    {
        mFrequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
            mProcessor.getVTSParameters(), frequencyParameterID.data(), mFrequencySlider));
        mFrequencySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
        mFrequencySlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);

        // Customize the text displayed in the textbox
        mFrequencySlider.textFromValueFunction = [](double value) {
            int intValue = static_cast<int>(std::ceil(value));

            return juce::String(intValue) + " HZ"; // Format the text with a suffix
        };
        mFrequencySlider.setLookAndFeel(&deathMetalLookAndFeel);
        mFrequencySlider.updateText();
        addAndMakeVisible(mFrequencySlider);
    }

    if (!gainParameterID.empty())
    {
        mGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mGainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
            mProcessor.getVTSParameters(), gainParameterID.data(), mGainSlider));
        mGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
        mGainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);

        // Customize the text displayed in the textbox
        mGainSlider.textFromValueFunction = [](double value) {
            return juce::String::formatted("%.1f DB", value); // Format the text with a suffix
        };
        mGainSlider.setLookAndFeel(&deathMetalLookAndFeel);
        mGainSlider.updateText();
        addAndMakeVisible(mGainSlider);
    }

    if (!QParameterID.empty())
    {
        mQSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mQAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(mProcessor.getVTSParameters(),
                                                                                    QParameterID.data(), mQSlider));
        mQSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
        mQSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);

        // Customize the text displayed in the textbox
        mQSlider.textFromValueFunction = [](double value) {
            return juce::String::formatted("%.3f", value); // Format the text with a suffix
        };

        mQSlider.setLookAndFeel(&deathMetalLookAndFeel);
        mQSlider.updateText();
        addAndMakeVisible(mQSlider);
    }
}

SliderGroup::~SliderGroup()
{
    mFrequencySlider.setLookAndFeel(nullptr);
    mGainSlider.setLookAndFeel(nullptr);
    mQSlider.setLookAndFeel(nullptr);
    filterSymbolLabel.setLookAndFeel(nullptr);
    frame.setLookAndFeel(nullptr);
}

void SliderGroup::resized()
{
    auto bounds = getLocalBounds();
    frame.setBounds(bounds);

    auto groupBounds = frame.getBounds().reduced(10, 20);

    filterSymbolLabel.setBounds(groupBounds.removeFromTop(groupBounds.getHeight() / 4));

    if (mQSlider.isVisible())
    {
        auto left = groupBounds.removeFromLeft(groupBounds.getWidth() / 2);
        mGainSlider.setBounds(left.removeFromTop(left.getHeight() / 2));
        mFrequencySlider.setBounds(left);
        mQSlider.setBounds(groupBounds.reduced(0, 30));
    }
    else if (mGainSlider.isVisible())
    {
        mGainSlider.setBounds(groupBounds.removeFromTop(groupBounds.getHeight() / 2));
        mFrequencySlider.setBounds(groupBounds);
    }
    else
    {
        mFrequencySlider.setBounds(groupBounds.reduced(20));
    }
}

void SliderGroup::setText(const juce::String &title)
{
    frame.setText(title);
}

void SliderGroup::setSymbolType(const juce::CharPointer_UTF8 &symbolType)
{
    filterSymbolLabel.setText(symbolType, juce::NotificationType::dontSendNotification);
}
