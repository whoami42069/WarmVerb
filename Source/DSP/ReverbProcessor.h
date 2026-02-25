#pragma once

#include <JuceHeader.h>
#include "CombFilter.h"
#include "AllpassFilter.h"
#include "ToneShaper.h"
#include <array>

class ReverbProcessor
{
public:
    struct Parameters
    {
        float roomSize = 0.5f;    // 0.0 - 1.0
        float damping = 0.5f;     // 0.0 - 1.0
        float width = 1.0f;       // 0.0 - 1.0
        float wetLevel = 0.33f;   // 0.0 - 1.0
        float dryLevel = 0.4f;    // 0.0 - 1.0
        float warmCold = 0.0f;    // -1.0 - 1.0
        float preDelay = 0.0f;    // 0 - 100 ms
        bool freeze = false;
    };

    ReverbProcessor() = default;

    void prepare(double sampleRate, int samplesPerBlock);
    void reset();
    void setParameters(const Parameters& params);
    void process(juce::AudioBuffer<float>& buffer);

private:
    void updateCombFilters();
    void updateAllpassFilters();

    // Freeverb delay times (in samples at 44100 Hz)
    static constexpr std::array<int, 8> combTuningL = { 1617, 1557, 1491, 1422, 1356, 1277, 1188, 1116 };
    static constexpr std::array<int, 4> allpassTuningL = { 556, 441, 341, 225 };
    static constexpr int stereoSpread = 23;

    // Freeverb scaling constants
    static constexpr float fixedGain = 0.015f;
    static constexpr float scaleWet = 3.0f;
    static constexpr float scaleDry = 2.0f;
    static constexpr float scaleDamp = 0.4f;
    static constexpr float scaleRoom = 0.28f;
    static constexpr float offsetRoom = 0.7f;

    // Filter banks (8 comb + 4 allpass per channel)
    std::array<CombFilter, 8> combFiltersL;
    std::array<CombFilter, 8> combFiltersR;
    std::array<AllpassFilter, 4> allpassFiltersL;
    std::array<AllpassFilter, 4> allpassFiltersR;

    // Tone shaper (warm/cold)
    ToneShaper toneShaper;

    // Pre-delay buffer
    juce::AudioBuffer<float> preDelayBuffer;
    int preDelayWriteIndex = 0;
    std::atomic<int> preDelaySamples { 0 };

    // Pre-allocated wet buffer (avoid allocation in audio thread)
    juce::AudioBuffer<float> wetBuffer;

    // Current parameters
    Parameters currentParams;
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;

    // Smoothed parameters
    juce::SmoothedValue<float> smoothedWet;
    juce::SmoothedValue<float> smoothedDry;
    juce::SmoothedValue<float> smoothedWidth;
};
