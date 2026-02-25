#pragma once

#include <JuceHeader.h>

namespace Colors
{
    const juce::Colour background      { 0xFF1A1A2E };  // Dark blue-gray
    const juce::Colour panelBackground { 0xFF252541 };  // Lighter panel
    const juce::Colour knobBackground  { 0xFF16213E };  // Knob fill
    const juce::Colour knobArc         { 0xFF0F4C75 };  // Inactive arc
    const juce::Colour knobAccent      { 0xFF3282B8 };  // Active arc (cyan)
    const juce::Colour warmAccent      { 0xFFE94560 };  // Warm indicator (red-pink)
    const juce::Colour coldAccent      { 0xFF00D9FF };  // Cold indicator (cyan)
    const juce::Colour textPrimary     { 0xFFEEEEEE };  // Main text
    const juce::Colour textSecondary   { 0xFF888888 };  // Labels
}

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel() override = default;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override;

    juce::Font getLabelFont(juce::Label& label) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};
