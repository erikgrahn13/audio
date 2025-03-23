#include "SettingsView.h"
#include "CustomLookAndFeel/CustomStandaloneFilterWindow.h"

SettingsView::SettingsView()
{
    // addAndMakeVisible(deviceTypeLabel);
    auto &deviceManager = juce::StandalonePluginHolder::getInstance()->deviceManager;
    const auto *audioDeviceType = deviceManager.getCurrentDeviceTypeObject();
    const auto &type = audioDeviceType->getDeviceNames();

    // Audio Device Type
    deviceTypeLabel.setJustificationType(juce::Justification::centredRight);
    deviceTypeLabel.setText("Driver Type:", juce::NotificationType::dontSendNotification);

    auto &availableDeviceTypes = deviceManager.getAvailableDeviceTypes();

    for (int i = 0; i < availableDeviceTypes.size(); ++i)
    {
        deviceTypeDropDown.addItem(availableDeviceTypes.getUnchecked(i)->getTypeName(), i + 1);
    }

    deviceTypeDropDown.setText(deviceManager.getCurrentAudioDeviceType(), dontSendNotification);

    addAndMakeVisible(deviceTypeDropDown);
    deviceTypeDropDown.onChange = [&] {
        if (auto *type = deviceManager.getAvailableDeviceTypes()[deviceTypeDropDown.getSelectedId() - 1])
        {
            // audioDeviceSettingsComp.reset();
            deviceManager.setCurrentAudioDeviceType(type->getTypeName(), true);
            deviceTypeDropDown.setText(deviceManager.getCurrentAudioDeviceType(), dontSendNotification);
            // updateAllControls(); // needed in case the type hasn't actually changed
        }
    };
    deviceTypeLabel.attachToComponent(&deviceTypeDropDown, true);

    // Output

    outputDeviceLabel.setText(audioDeviceType->hasSeparateInputsAndOutputs() ? "Output Device:" : "Device",
                              juce::NotificationType::dontSendNotification);
    outputDeviceLabel.attachToComponent(&outputDeviceDropDown, true);
    outputDeviceDropDown.onChange = [&] {
        auto config = deviceManager.getAudioDeviceSetup();
        config.outputDeviceName =
            outputDeviceDropDown.getSelectedId() < 0 ? juce::String() : outputDeviceDropDown.getText();
        deviceManager.setAudioDeviceSetup(config, true);
    };
    auto outputDeviceIndex = type.indexOf(deviceManager.getAudioDeviceSetup().outputDeviceName);

    for (int i = 0; i < type.size(); ++i)
    {
        outputDeviceDropDown.addItem(type[i], i + 1);
    }
    outputDeviceDropDown.setSelectedId(outputDeviceIndex + 1);
    addAndMakeVisible(outputDeviceDropDown);

    // Input
    if (audioDeviceType->hasSeparateInputsAndOutputs())
    {
        inputDeviceLabel.setText("Input Device", juce::NotificationType::dontSendNotification);
        inputDeviceLabel.attachToComponent(&inputDeviceDropDown, true);
        inputDeviceDropDown.onChange = [&] {
            auto config = deviceManager.getAudioDeviceSetup();
            config.inputDeviceName =
                inputDeviceDropDown.getSelectedId() < 0 ? juce::String() : inputDeviceDropDown.getText();
            deviceManager.setAudioDeviceSetup(config, true);
        };
        // const auto type = deviceManager.getCurrentDeviceTypeObject()->getDeviceNames();
        auto inputDeviceIndex = type.indexOf(deviceManager.getAudioDeviceSetup().inputDeviceName);

        for (int i = 0; i < type.size(); ++i)
        {
            inputDeviceDropDown.addItem(type[i], i + 1);
        }
        inputDeviceDropDown.setSelectedId(inputDeviceIndex + 1);
        addAndMakeVisible(inputDeviceDropDown);
    }

    outputChannelsLabel.setText("Output channels", juce::NotificationType::dontSendNotification);
    outputChannelsLabel.attachToComponent(&outputChannelsLeftDropDown, true);
    auto outputChannels = deviceManager.getCurrentAudioDevice()->getOutputChannelNames();
    for (int i = 0; i < outputChannels.size(); ++i)
    {
        outputChannelsLeftDropDown.addItem(outputChannels[i], i + 1);
    }
    outputChannelsLeftDropDown.setSelectedId(1);
    addAndMakeVisible(outputChannelsLeftDropDown);

    // outputChannelsLabel.attachToComponent(&outputChannelsRightDropDown, true);
    // auto outputChannels = deviceManager.getCurrentAudioDevice()->getOutputChannelNames();
    for (int i = 0; i < outputChannels.size(); ++i)
    {
        outputChannelsRightDropDown.addItem(outputChannels[i], i + 1);
    }
    outputChannelsRightDropDown.setSelectedId(1);
    addAndMakeVisible(outputChannelsRightDropDown);
    // auto hej = deviceManager.getCurrentAudioDevice();
    // std::ignore = hej;
}

SettingsView::~SettingsView()
{
}

void SettingsView::paint(juce::Graphics &g)
{
    std::ignore = g;
}

void SettingsView::resized()
{
    auto itemHeight{24};
    auto bounds = getLocalBounds();
    // deviceTypeLabel.centreWithSize(bounds.getWidth(), bounds.getHeight());
    Rectangle<int> r(proportionOfWidth(0.35f), 15, proportionOfWidth(0.6f), 3000);
    auto space = itemHeight / 4;

    deviceTypeDropDown.setBounds(r.removeFromTop(itemHeight));
    r.removeFromTop(space * 3);

    outputDeviceDropDown.setBounds(r.removeFromTop(itemHeight));
    r.removeFromTop(space);

    inputDeviceDropDown.setBounds(r.removeFromTop(itemHeight));
    r.removeFromTop(space);

    auto tmp = r.removeFromTop(itemHeight);
    outputChannelsLeftDropDown.setBounds(tmp.removeFromLeft(tmp.getWidth() / 2));
    // r.removeFromTop(space);

    outputChannelsRightDropDown.setBounds(tmp);
    r.removeFromTop(space);
}
