#pragma once

#include <JuceHeader.h>

class ReverbKnob : public juce::Slider
{
public:
    ReverbKnob();
    ~ReverbKnob() override = default;

    void setLabel(const juce::String& labelText);

private:
    juce::String label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbKnob)
};
