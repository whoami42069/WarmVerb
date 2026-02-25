#pragma once

#include <JuceHeader.h>

namespace ParamID
{
    // Core reverb parameters
    inline constexpr auto roomSize  = "roomSize";
    inline constexpr auto damping   = "damping";
    inline constexpr auto width     = "width";
    inline constexpr auto wetLevel  = "wetLevel";
    inline constexpr auto dryLevel  = "dryLevel";

    // Character parameter (warm/cold)
    inline constexpr auto warmCold  = "warmCold";

    // Additional controls
    inline constexpr auto preDelay  = "preDelay";
    inline constexpr auto freeze    = "freeze";
}

namespace ParameterDefaults
{
    inline constexpr float roomSize  = 0.5f;
    inline constexpr float damping   = 0.5f;
    inline constexpr float width     = 1.0f;
    inline constexpr float wetLevel  = 0.33f;
    inline constexpr float dryLevel  = 0.4f;
    inline constexpr float warmCold  = 0.0f;
    inline constexpr float preDelay  = 0.0f;
    inline constexpr bool  freeze    = false;
}
