#include "PresetManager.h"
#include "../Utils/ParameterIDs.h"

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts)
    : valueTreeState(apvts)
{
    // Load first factory preset as default
    if (FactoryPresets::getNumPresets() > 0)
    {
        loadPreset(0);
    }
}

juce::File PresetManager::getPresetDirectory() const
{
    // Get application data directory
    auto dir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                   .getChildFile("WarmVerb")
                   .getChildFile("Presets");

    // Create directory if it doesn't exist
    if (!dir.exists())
    {
        dir.createDirectory();
    }

    return dir;
}

void PresetManager::savePreset(const juce::String& presetName)
{
    auto presetFile = getPresetDirectory().getChildFile(presetName + ".xml");

    // Get current state
    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    if (xml != nullptr)
    {
        xml->setAttribute("presetName", presetName);
        xml->writeTo(presetFile);
    }

    currentPresetName = presetName;
}

void PresetManager::loadPreset(int presetIndex)
{
    auto allPresets = getAllPresets();

    if (presetIndex < 0 || presetIndex >= allPresets.size())
        return;

    currentPresetIndex = presetIndex;

    // Check if it's a factory preset
    if (presetIndex < FactoryPresets::getNumPresets())
    {
        loadPresetData(FactoryPresets::getPreset(presetIndex));
        currentPresetName = FactoryPresets::getPreset(presetIndex).name;
    }
    else
    {
        // Load user preset
        int userIndex = presetIndex - FactoryPresets::getNumPresets();
        auto userPresets = scanForUserPresets();

        if (userIndex < userPresets.size())
        {
            loadPreset(userPresets[userIndex]);
        }
    }
}

void PresetManager::loadPreset(const juce::String& presetName)
{
    // Check factory presets first
    for (int i = 0; i < FactoryPresets::getNumPresets(); ++i)
    {
        if (FactoryPresets::getPreset(i).name == presetName)
        {
            loadPresetData(FactoryPresets::getPreset(i));
            currentPresetName = presetName;
            currentPresetIndex = i;
            return;
        }
    }

    // Check user presets
    auto presetFile = getPresetDirectory().getChildFile(presetName + ".xml");

    if (presetFile.existsAsFile())
    {
        std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(presetFile);

        if (xml != nullptr && xml->hasTagName(valueTreeState.state.getType()))
        {
            valueTreeState.replaceState(juce::ValueTree::fromXml(*xml));
            currentPresetName = presetName;
        }
    }
}

void PresetManager::loadPresetData(const PresetData& preset)
{
    if (auto* param = valueTreeState.getParameter(ParamID::roomSize))
        param->setValueNotifyingHost(preset.roomSize);

    if (auto* param = valueTreeState.getParameter(ParamID::damping))
        param->setValueNotifyingHost(preset.damping);

    if (auto* param = valueTreeState.getParameter(ParamID::width))
        param->setValueNotifyingHost(preset.width);

    if (auto* param = valueTreeState.getParameter(ParamID::wetLevel))
        param->setValueNotifyingHost(preset.wetLevel);

    if (auto* param = valueTreeState.getParameter(ParamID::dryLevel))
        param->setValueNotifyingHost(preset.dryLevel);

    // warmCold needs special handling as it's -1 to 1, but normalized is 0 to 1
    if (auto* param = valueTreeState.getParameter(ParamID::warmCold))
        param->setValueNotifyingHost((preset.warmCold + 1.0f) / 2.0f);

    if (auto* param = valueTreeState.getParameter(ParamID::preDelay))
        param->setValueNotifyingHost(preset.preDelay / 100.0f); // Normalized

    if (auto* param = valueTreeState.getParameter(ParamID::freeze))
        param->setValueNotifyingHost(preset.freeze ? 1.0f : 0.0f);
}

void PresetManager::deletePreset(const juce::String& presetName)
{
    // Only delete user presets, not factory presets
    for (int i = 0; i < FactoryPresets::getNumPresets(); ++i)
    {
        if (FactoryPresets::getPreset(i).name == presetName)
            return; // Can't delete factory preset
    }

    auto presetFile = getPresetDirectory().getChildFile(presetName + ".xml");
    if (presetFile.existsAsFile())
    {
        presetFile.deleteFile();
    }
}

int PresetManager::loadNextPreset()
{
    auto allPresets = getAllPresets();
    if (allPresets.isEmpty())
        return 0;

    currentPresetIndex = (currentPresetIndex + 1) % allPresets.size();
    loadPreset(currentPresetIndex);
    return currentPresetIndex;
}

int PresetManager::loadPreviousPreset()
{
    auto allPresets = getAllPresets();
    if (allPresets.isEmpty())
        return 0;

    currentPresetIndex = (currentPresetIndex - 1 + allPresets.size()) % allPresets.size();
    loadPreset(currentPresetIndex);
    return currentPresetIndex;
}

juce::StringArray PresetManager::getAllPresets() const
{
    juce::StringArray presets;

    // Add factory presets
    for (int i = 0; i < FactoryPresets::getNumPresets(); ++i)
    {
        presets.add(FactoryPresets::getPreset(i).name);
    }

    // Add user presets
    presets.addArray(scanForUserPresets());

    return presets;
}

juce::StringArray PresetManager::scanForUserPresets() const
{
    juce::StringArray userPresets;

    auto presetDir = getPresetDirectory();
    auto files = presetDir.findChildFiles(juce::File::findFiles, false, "*.xml");

    for (const auto& file : files)
    {
        userPresets.add(file.getFileNameWithoutExtension());
    }

    userPresets.sort(true);
    return userPresets;
}

juce::String PresetManager::getCurrentPreset() const
{
    return currentPresetName;
}
