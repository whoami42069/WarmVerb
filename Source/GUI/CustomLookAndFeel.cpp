#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    setColour(juce::Slider::textBoxTextColourId, Colors::textPrimary);
    setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour(juce::Label::textColourId, Colors::textSecondary);
    setColour(juce::ComboBox::backgroundColourId, Colors::panelBackground);
    setColour(juce::ComboBox::textColourId, Colors::textPrimary);
    setColour(juce::ComboBox::outlineColourId, Colors::knobArc);
    setColour(juce::PopupMenu::backgroundColourId, Colors::panelBackground);
    setColour(juce::PopupMenu::textColourId, Colors::textPrimary);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, Colors::knobAccent);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                         juce::Slider& slider)
{
    juce::ignoreUnused(slider);

    const float radius = static_cast<float>(juce::jmin(width / 2, height / 2)) - 4.0f;
    const float centreX = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
    const float centreY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Draw background circle
    g.setColour(Colors::knobBackground);
    g.fillEllipse(rx, ry, rw, rw);

    // Draw outer ring
    g.setColour(Colors::knobArc);
    g.drawEllipse(rx, ry, rw, rw, 2.0f);

    // Draw value arc
    juce::Path valueArc;
    valueArc.addCentredArc(centreX, centreY, radius - 4.0f, radius - 4.0f,
                           0.0f, rotaryStartAngle, angle, true);
    g.setColour(Colors::knobAccent);
    g.strokePath(valueArc, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved,
                                                 juce::PathStrokeType::rounded));

    // Draw pointer
    juce::Path pointer;
    const float pointerLength = radius * 0.6f;
    const float pointerThickness = 3.0f;
    pointer.addRectangle(-pointerThickness * 0.5f, -radius + 6.0f, pointerThickness, pointerLength);
    pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    g.setColour(Colors::textPrimary);
    g.fillPath(pointer);

    // Draw center dot
    g.setColour(Colors::knobAccent);
    g.fillEllipse(centreX - 4.0f, centreY - 4.0f, 8.0f, 8.0f);
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float minSliderPos, float maxSliderPos,
                                         juce::Slider::SliderStyle style, juce::Slider& slider)
{
    juce::ignoreUnused(minSliderPos, maxSliderPos, style);

    const float trackHeight = 6.0f;
    const float trackY = static_cast<float>(y) + (static_cast<float>(height) - trackHeight) * 0.5f;
    const float trackWidth = static_cast<float>(width);
    const float thumbRadius = 10.0f;

    // Calculate center position for warm/cold gradient
    const float centerX = static_cast<float>(x) + trackWidth * 0.5f;

    // Draw track background
    juce::Rectangle<float> track(static_cast<float>(x), trackY, trackWidth, trackHeight);
    g.setColour(Colors::knobArc);
    g.fillRoundedRectangle(track, trackHeight * 0.5f);

    // Determine if we're dealing with a centered slider (warm/cold)
    bool isCentered = slider.getMinimum() < 0 && slider.getMaximum() > 0;

    if (isCentered)
    {
        // Draw from center to current position
        float fillStart = centerX;
        float fillEnd = sliderPos;

        if (sliderPos < centerX)
        {
            // Cold side (cyan gradient)
            juce::ColourGradient coldGradient(Colors::knobAccent, fillStart, trackY,
                                               Colors::coldAccent, fillEnd, trackY, false);
            g.setGradientFill(coldGradient);
        }
        else
        {
            // Warm side (red gradient)
            juce::ColourGradient warmGradient(Colors::knobAccent, fillStart, trackY,
                                               Colors::warmAccent, fillEnd, trackY, false);
            g.setGradientFill(warmGradient);
        }

        float fillX = juce::jmin(fillStart, fillEnd);
        float fillWidth = std::abs(fillEnd - fillStart);
        g.fillRoundedRectangle(fillX, trackY, fillWidth, trackHeight, trackHeight * 0.5f);
    }
    else
    {
        // Standard linear fill
        juce::Rectangle<float> fill(static_cast<float>(x), trackY, sliderPos - static_cast<float>(x), trackHeight);
        g.setColour(Colors::knobAccent);
        g.fillRoundedRectangle(fill, trackHeight * 0.5f);
    }

    // Draw thumb
    g.setColour(Colors::textPrimary);
    g.fillEllipse(sliderPos - thumbRadius, trackY + trackHeight * 0.5f - thumbRadius,
                  thumbRadius * 2.0f, thumbRadius * 2.0f);

    g.setColour(Colors::knobAccent);
    g.drawEllipse(sliderPos - thumbRadius, trackY + trackHeight * 0.5f - thumbRadius,
                  thumbRadius * 2.0f, thumbRadius * 2.0f, 2.0f);
}

void CustomLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                         bool shouldDrawButtonAsHighlighted,
                                         bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    auto bounds = button.getLocalBounds().toFloat().reduced(2.0f);

    // Background
    g.setColour(button.getToggleState() ? Colors::knobAccent : Colors::knobArc);
    g.fillRoundedRectangle(bounds, 6.0f);

    // Border
    g.setColour(button.getToggleState() ? Colors::knobAccent.brighter(0.2f) : Colors::panelBackground);
    g.drawRoundedRectangle(bounds, 6.0f, 2.0f);

    // Text
    g.setColour(Colors::textPrimary);
    g.setFont(juce::Font(14.0f, juce::Font::bold));
    g.drawText(button.getButtonText(), bounds, juce::Justification::centred);
}

juce::Font CustomLookAndFeel::getLabelFont(juce::Label& label)
{
    juce::ignoreUnused(label);
    return juce::Font(12.0f);
}
