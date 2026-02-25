#pragma once

#include <JuceHeader.h>

namespace VersionInfo
{
    inline constexpr int MAJOR = 1;
    inline constexpr int MINOR = 0;
    inline constexpr int PATCH = 0;

    inline juce::String getVersionString()
    {
        return juce::String(MAJOR) + "." + juce::String(MINOR) + "." + juce::String(PATCH);
    }

    // Placeholder for future update checking
    inline void checkForUpdates()
    {
        // TODO: Implement version check against remote endpoint
        // Example: fetch JSON from https://warmverb.com/api/version
        // Compare with current version
        // Show update dialog if newer version available
    }

    inline bool isUpdateAvailable()
    {
        // TODO: Implement actual check
        return false;
    }
}
