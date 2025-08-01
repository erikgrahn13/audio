#include "SliderGroup.h"

SliderGroup::SliderGroup(AudioPluginAudioProcessor &processor, std::string_view bypassParameterID,
                         std::string_view frequencyParameterID, std::string_view gainParameterID,
                         std::string_view QParameterID, std::string_view filterOrderParameterID)
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

    if (!bypassParameterID.empty())
    {
        mBypassAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
            mProcessor.mParameters, bypassParameterID.data(), mBypassButton));

        mBypassButton.setLookAndFeel(&fontAudioLookAndFeel);
        mBypassButton.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::white);
        mBypassButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::grey);
        mBypassButton.setButtonText(juce::CharPointer_UTF8(""));
        mBypassButton.setClickingTogglesState(true);
        mBypassButton.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
        mBypassButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::transparentBlack);
        addAndMakeVisible(mBypassButton);
    }

    if (!frequencyParameterID.empty())
    {
        mFrequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
            mProcessor.mParameters, frequencyParameterID.data(), mFrequencySlider));
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
            mProcessor.mParameters, gainParameterID.data(), mGainSlider));
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
    else
    {
        auto *filterOrder = dynamic_cast<juce::AudioParameterChoice *>(
            mProcessor.mParameters.getParameter(filterOrderParameterID.data()));

        for (int i = 0; i < filterOrder->choices.size(); ++i)
        {
            mFilterOrder.addItem(filterOrder->choices[i], i + 1);
        }

        mFilterOrder.setSelectedId(filterOrder->getIndex());
        mFilterOrder.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::white);
        mFilterOrder.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black);
        mFilterOrder.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::black);
        mFilterOrder.setLookAndFeel(&deathMetalLookAndFeel);
        mFilterOrder.setJustificationType(juce::Justification::centred);
        std::ignore = filterOrderParameterID;
        mFilterOrderAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(
            mProcessor.mParameters, filterOrderParameterID.data(), mFilterOrder));
        addAndMakeVisible(mFilterOrder);
    }

    if (!QParameterID.empty())
    {
        mQSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mQAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(mProcessor.mParameters,
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
    mBypassButton.setLookAndFeel(nullptr);
    mFilterOrder.setLookAndFeel(nullptr);
}

void SliderGroup::resized()
{
    auto bounds = getLocalBounds();
    frame.setBounds(bounds);

    auto groupBounds = frame.getBounds().reduced(10, 20);

    auto labels = groupBounds.removeFromTop(groupBounds.getHeight() / 4);
    mBypassButton.setBounds(labels.removeFromLeft(labels.getWidth() / 2).reduced(15, 10));
    filterSymbolLabel.setBounds(labels);

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

        auto filterOrder = groupBounds.removeFromTop(groupBounds.getHeight() / 4);
        mFilterOrder.setBounds(filterOrder);
        mFrequencySlider.setBounds(groupBounds.reduced(10));
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
