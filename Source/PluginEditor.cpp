#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils/ParameterIDs.h"

WarmVerbAudioProcessorEditor::WarmVerbAudioProcessorEditor(WarmVerbAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setLookAndFeel(&customLookAndFeel);

    // Setup knobs
    setupKnob(roomSizeKnob, roomSizeLabel, "Size");
    setupKnob(dampingKnob, dampingLabel, "Damping");
    setupKnob(wetKnob, wetLabel, "Wet");
    setupKnob(dryKnob, dryLabel, "Dry");
    setupKnob(widthKnob, widthLabel, "Width");
    setupKnob(preDelayKnob, preDelayLabel, "Pre-Delay");

    // Setup character slider
    setupSlider(characterSlider);
    characterSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    characterSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    // Character labels
    characterLabel.setText("Character", juce::dontSendNotification);
    characterLabel.setJustificationType(juce::Justification::centred);
    characterLabel.setColour(juce::Label::textColourId, juce::Colour(0xFF888888));
    addAndMakeVisible(characterLabel);

    coldLabel.setText("COLD", juce::dontSendNotification);
    coldLabel.setJustificationType(juce::Justification::centredLeft);
    coldLabel.setColour(juce::Label::textColourId, juce::Colour(0xFF00D9FF));
    addAndMakeVisible(coldLabel);

    warmLabel.setText("WARM", juce::dontSendNotification);
    warmLabel.setJustificationType(juce::Justification::centredRight);
    warmLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFE94560));
    addAndMakeVisible(warmLabel);

    // Freeze button
    freezeButton.setButtonText("FREEZE");
    freezeButton.setColour(juce::ToggleButton::textColourId, juce::Colour(0xFFEEEEEE));
    freezeButton.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xFF3282B8));
    addAndMakeVisible(freezeButton);

    // Parameter attachments
    auto& apvts = audioProcessor.getAPVTS();
    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, ParamID::roomSize, roomSizeKnob);
    dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, ParamID::damping, dampingKnob);
    wetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, ParamID::wetLevel, wetKnob);
    dryAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, ParamID::dryLevel, dryKnob);
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, ParamID::width, widthKnob);
    preDelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, ParamID::preDelay, preDelayKnob);
    characterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, ParamID::warmCold, characterSlider);
    freezeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, ParamID::freeze, freezeButton);

    setSize(600, 400);
}

WarmVerbAudioProcessorEditor::~WarmVerbAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void WarmVerbAudioProcessorEditor::setupKnob(juce::Slider& knob, juce::Label& label, const juce::String& labelText)
{
    knob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    knob.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFEEEEEE));
    knob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(knob);

    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colour(0xFF888888));
    addAndMakeVisible(label);
}

void WarmVerbAudioProcessorEditor::setupSlider(juce::Slider& slider)
{
    slider.setColour(juce::Slider::trackColourId, juce::Colour(0xFF0F4C75));
    slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xFF3282B8));
    addAndMakeVisible(slider);
}

void WarmVerbAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(0xFF1A1A2E));

    // Header
    g.setColour(juce::Colour(0xFF252541));
    g.fillRect(0, 0, getWidth(), 50);

    // Logo/Title
    g.setColour(juce::Colour(0xFFEEEEEE));
    g.setFont(juce::Font(24.0f, juce::Font::bold));
    g.drawText("WarmVerb", 20, 10, 200, 30, juce::Justification::centredLeft);

    // Version
    g.setFont(juce::Font(12.0f));
    g.setColour(juce::Colour(0xFF888888));
    g.drawText("v1.0.0", 20, 32, 60, 15, juce::Justification::centredLeft);

    // Section dividers
    g.setColour(juce::Colour(0xFF252541));
    g.fillRect(0, 200, getWidth(), 2);
    g.fillRect(0, 280, getWidth(), 2);
}

void WarmVerbAudioProcessorEditor::resized()
{
    const int knobSize = 80;
    const int knobY = 80;
    const int labelHeight = 20;
    const int spacing = 130;
    const int startX = 45;

    // Main knobs row (Size, Damping, Wet, Dry)
    roomSizeKnob.setBounds(startX, knobY, knobSize, knobSize);
    roomSizeLabel.setBounds(startX, knobY + knobSize + 5, knobSize, labelHeight);

    dampingKnob.setBounds(startX + spacing, knobY, knobSize, knobSize);
    dampingLabel.setBounds(startX + spacing, knobY + knobSize + 5, knobSize, labelHeight);

    wetKnob.setBounds(startX + spacing * 2, knobY, knobSize, knobSize);
    wetLabel.setBounds(startX + spacing * 2, knobY + knobSize + 5, knobSize, labelHeight);

    dryKnob.setBounds(startX + spacing * 3, knobY, knobSize, knobSize);
    dryLabel.setBounds(startX + spacing * 3, knobY + knobSize + 5, knobSize, labelHeight);

    // Character slider row
    const int sliderY = 220;
    coldLabel.setBounds(30, sliderY, 50, 25);
    characterSlider.setBounds(90, sliderY, getWidth() - 180, 25);
    warmLabel.setBounds(getWidth() - 80, sliderY, 50, 25);
    characterLabel.setBounds(0, sliderY + 30, getWidth(), labelHeight);

    // Bottom row (Width, Pre-Delay, Freeze)
    const int bottomY = 300;
    const int smallKnobSize = 60;

    widthKnob.setBounds(40, bottomY, smallKnobSize, smallKnobSize);
    widthLabel.setBounds(40, bottomY + smallKnobSize + 5, smallKnobSize, labelHeight);

    preDelayKnob.setBounds(140, bottomY, smallKnobSize, smallKnobSize);
    preDelayLabel.setBounds(140, bottomY + smallKnobSize + 5, smallKnobSize, labelHeight);

    freezeButton.setBounds(getWidth() - 120, bottomY + 20, 100, 30);
}
