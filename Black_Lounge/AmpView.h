#pragma once

#include <JuceHeader.h>

class AmpView : public juce::Component
{
  public:
    AmpView();
    ~AmpView();
    void paint(juce::Graphics &g) override;
    void resized() override;

  private:
    juce::Label testLabel;
};