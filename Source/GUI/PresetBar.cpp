#include "PresetBar.h"

PresetBar::PresetBar(PresetManager& presetMgr)
    : presetManager(presetMgr)
{
    // Preset selector
    presetSelector.setTextWhenNothingSelected("Select Preset...");
    presetSelector.onChange = [this] { loadSelectedPreset(); };
    addAndMakeVisible(presetSelector);

    // Previous button
    prevButton.setButtonText("<");
    prevButton.addListener(this);
    addAndMakeVisible(prevButton);

    // Next button
    nextButton.setButtonText(">");
    nextButton.addListener(this);
    addAndMakeVisible(nextButton);

    // Save button
    saveButton.setButtonText("Save");
    saveButton.addListener(this);
    addAndMakeVisible(saveButton);

    updatePresetList();
}

PresetBar::~PresetBar()
{
    prevButton.removeListener(this);
    nextButton.removeListener(this);
    saveButton.removeListener(this);
}

void PresetBar::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void PresetBar::resized()
{
    auto bounds = getLocalBounds().reduced(5);

    const int buttonWidth = 30;
    const int saveWidth = 50;
    const int spacing = 5;

    prevButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);

    nextButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);

    saveButton.setBounds(bounds.removeFromRight(saveWidth));
    bounds.removeFromRight(spacing);

    presetSelector.setBounds(bounds);
}

void PresetBar::buttonClicked(juce::Button* button)
{
    if (button == &prevButton)
    {
        int newIndex = presetManager.loadPreviousPreset();
        presetSelector.setSelectedId(newIndex + 1, juce::dontSendNotification);
    }
    else if (button == &nextButton)
    {
        int newIndex = presetManager.loadNextPreset();
        presetSelector.setSelectedId(newIndex + 1, juce::dontSendNotification);
    }
    else if (button == &saveButton)
    {
        // Show save dialog
        auto dialog = std::make_unique<juce::AlertWindow>("Save Preset",
                                                          "Enter preset name:",
                                                          juce::MessageBoxIconType::NoIcon);
        dialog->addTextEditor("presetName", presetManager.getCurrentPreset(), "Name:");
        dialog->addButton("Save", 1);
        dialog->addButton("Cancel", 0);

        dialog->enterModalState(true,
            juce::ModalCallbackFunction::create([this](int result)
            {
                if (result == 1)
                {
                    // Get the parent window's alert window
                    if (auto* alertWindow = dynamic_cast<juce::AlertWindow*>(
                            juce::Component::getCurrentlyModalComponent()))
                    {
                        auto name = alertWindow->getTextEditorContents("presetName");
                        if (name.isNotEmpty())
                        {
                            presetManager.savePreset(name);
                            updatePresetList();
                        }
                    }
                }
            }),
            true);
    }
}

void PresetBar::loadSelectedPreset()
{
    int selectedIndex = presetSelector.getSelectedId() - 1;
    if (selectedIndex >= 0)
    {
        presetManager.loadPreset(selectedIndex);
    }
}

void PresetBar::updatePresetList()
{
    presetSelector.clear();

    auto presets = presetManager.getAllPresets();
    int id = 1;
    for (const auto& preset : presets)
    {
        presetSelector.addItem(preset, id++);
    }

    // Select current preset
    auto currentPreset = presetManager.getCurrentPreset();
    for (int i = 0; i < presetSelector.getNumItems(); ++i)
    {
        if (presetSelector.getItemText(i) == currentPreset)
        {
            presetSelector.setSelectedId(i + 1, juce::dontSendNotification);
            break;
        }
    }
}
