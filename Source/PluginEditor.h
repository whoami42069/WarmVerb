#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/CustomLookAndFeel.h"

class WarmVerbAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit WarmVerbAudioProcessorEditor(WarmVerbAudioProcessor&);
    ~WarmVerbAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    WarmVerbAudioProcessor& audioProcessor;

    CustomLookAndFeel customLookAndFeel;

    // Knobs
    juce::Slider roomSizeKnob;
    juce::Slider dampingKnob;
    juce::Slider wetKnob;
    juce::Slider dryKnob;
    juce::Slider widthKnob;
    juce::Slider preDelayKnob;

    // Character slider (warm/cold)
    juce::Slider characterSlider;

    // Freeze button
    juce::ToggleButton freezeButton;

    // Labels
    juce::Label roomSizeLabel;
    juce::Label dampingLabel;
    juce::Label wetLabel;
    juce::Label dryLabel;
    juce::Label widthLabel;
    juce::Label preDelayLabel;
    juce::Label characterLabel;
    juce::Label coldLabel;
    juce::Label warmLabel;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preDelayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> characterAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> freezeAttachment;

    void setupKnob(juce::Slider& knob, juce::Label& label, const juce::String& labelText);
    void setupSlider(juce::Slider& slider);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WarmVerbAudioProcessorEditor)
};
