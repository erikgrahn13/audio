#include "AmpView.h"

AmpView::AmpView()
{
    testLabel.setText("AmpView", juce::NotificationType::dontSendNotification);
    testLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(testLabel);
}

AmpView::~AmpView()
{
}

void AmpView::paint(juce::Graphics &g)
{
    std::ignore = g;
}

void AmpView::resized()
{
    auto bounds = getLocalBounds();
    testLabel.centreWithSize(bounds.getWidth(), bounds.getHeight());
}
