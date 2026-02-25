#pragma once

#include <JuceHeader.h>
#include "../Presets/PresetManager.h"

class PresetBar : public juce::Component,
                  public juce::Button::Listener
{
public:
    PresetBar(PresetManager& presetMgr);
    ~PresetBar() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;

    void updatePresetList();

private:
    PresetManager& presetManager;

    juce::ComboBox presetSelector;
    juce::TextButton prevButton;
    juce::TextButton nextButton;
    juce::TextButton saveButton;

    void loadSelectedPreset();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetBar)
};
