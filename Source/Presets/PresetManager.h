#pragma once

#include <JuceHeader.h>
#include "FactoryPresets.h"

class PresetManager
{
public:
    PresetManager(juce::AudioProcessorValueTreeState& apvts);

    // Core operations
    void savePreset(const juce::String& presetName);
    void loadPreset(int presetIndex);
    void loadPreset(const juce::String& presetName);
    void deletePreset(const juce::String& presetName);

    // Navigation
    int loadNextPreset();
    int loadPreviousPreset();

    // Access
    juce::StringArray getAllPresets() const;
    juce::String getCurrentPreset() const;
    int getCurrentPresetIndex() const { return currentPresetIndex; }

private:
    juce::AudioProcessorValueTreeState& valueTreeState;
    int currentPresetIndex = 0;
    juce::String currentPresetName;

    juce::File getPresetDirectory() const;
    void loadPresetData(const PresetData& preset);
    juce::StringArray scanForUserPresets() const;
};
