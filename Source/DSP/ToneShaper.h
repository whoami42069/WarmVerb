#pragma once

#include <JuceHeader.h>

class ToneShaper
{
public:
    ToneShaper() = default;

    void prepare(double sampleRate, int samplesPerBlock)
    {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
        spec.numChannels = 2;

        lowShelf.prepare(spec);
        highShelf.prepare(spec);

        currentSampleRate = sampleRate;
        updateFilters();
    }

    void setTone(float warmCold)
    {
        // warmCold: -1.0 (cold) to +1.0 (warm)
        currentTone = juce::jlimit(-1.0f, 1.0f, warmCold);
        updateFilters();
    }

    void process(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);

        lowShelf.process(context);
        highShelf.process(context);
    }

    void reset()
    {
        lowShelf.reset();
        highShelf.reset();
    }

private:
    void updateFilters()
    {
        if (currentSampleRate <= 0.0)
            return;

        // Maximum gain change: +/- 6dB
        const float maxGainDb = 6.0f;

        // Warm: boost lows, cut highs
        // Cold: cut lows, boost highs
        float lowGainDb = currentTone * maxGainDb;
        float highGainDb = -currentTone * maxGainDb;

        // Low shelf at 200 Hz
        *lowShelf.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
            currentSampleRate,
            200.0f,
            0.707f,
            juce::Decibels::decibelsToGain(lowGainDb));

        // High shelf at 4000 Hz
        *highShelf.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
            currentSampleRate,
            4000.0f,
            0.707f,
            juce::Decibels::decibelsToGain(highGainDb));
    }

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowShelf;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highShelf;

    double currentSampleRate = 44100.0;
    float currentTone = 0.0f;
};
