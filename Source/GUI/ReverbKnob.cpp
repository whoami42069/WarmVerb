#include "ReverbKnob.h"

ReverbKnob::ReverbKnob()
{
    setSliderStyle(juce::Slider::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
}

void ReverbKnob::setLabel(const juce::String& labelText)
{
    label = labelText;
}
