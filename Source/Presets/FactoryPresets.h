#pragma once

#include <JuceHeader.h>
#include <array>

struct PresetData
{
    juce::String name;
    float roomSize;
    float damping;
    float width;
    float wetLevel;
    float dryLevel;
    float warmCold;
    float preDelay;
    bool freeze;
};

namespace FactoryPresets
{
    inline const std::array<PresetData, 5> presets = {{
        // Small Room - Tight, controlled reverb
        {
            "Small Room",
            0.3f,   // roomSize
            0.7f,   // damping
            0.8f,   // width
            0.25f,  // wetLevel
            0.5f,   // dryLevel
            0.0f,   // warmCold
            10.0f,  // preDelay
            false   // freeze
        },
        // Medium Hall - Balanced all-purpose
        {
            "Medium Hall",
            0.6f,   // roomSize
            0.5f,   // damping
            1.0f,   // width
            0.35f,  // wetLevel
            0.4f,   // dryLevel
            0.0f,   // warmCold
            20.0f,  // preDelay
            false   // freeze
        },
        // Large Hall - Spacious, long decay
        {
            "Large Hall",
            0.85f,  // roomSize
            0.3f,   // damping
            1.0f,   // width
            0.4f,   // wetLevel
            0.35f,  // dryLevel
            0.0f,   // warmCold
            40.0f,  // preDelay
            false   // freeze
        },
        // Warm Plate - Vintage warm character
        {
            "Warm Plate",
            0.7f,   // roomSize
            0.6f,   // damping
            0.5f,   // width
            0.4f,   // wetLevel
            0.3f,   // dryLevel
            0.6f,   // warmCold (warm)
            5.0f,   // preDelay
            false   // freeze
        },
        // Shimmer - Bright, airy texture
        {
            "Shimmer",
            0.9f,   // roomSize
            0.2f,   // damping
            1.0f,   // width
            0.5f,   // wetLevel
            0.25f,  // dryLevel
            -0.5f,  // warmCold (cold/bright)
            30.0f,  // preDelay
            false   // freeze
        }
    }};

    inline int getNumPresets()
    {
        return static_cast<int>(presets.size());
    }

    inline const PresetData& getPreset(int index)
    {
        return presets[static_cast<size_t>(juce::jlimit(0, getNumPresets() - 1, index))];
    }
}
