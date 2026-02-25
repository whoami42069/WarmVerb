#include "ReverbProcessor.h"

void ReverbProcessor::prepare(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;

    const double sampleRateRatio = sampleRate / 44100.0;

    // Initialize comb filters with scaled delay times
    for (int i = 0; i < 8; ++i)
    {
        int delaySamplesL = static_cast<int>(combTuningL[i] * sampleRateRatio);
        int delaySamplesR = static_cast<int>((combTuningL[i] + stereoSpread) * sampleRateRatio);

        combFiltersL[i].setBufferSize(delaySamplesL);
        combFiltersR[i].setBufferSize(delaySamplesR);
    }

    // Initialize allpass filters with scaled delay times
    for (int i = 0; i < 4; ++i)
    {
        int delaySamplesL = static_cast<int>(allpassTuningL[i] * sampleRateRatio);
        int delaySamplesR = static_cast<int>((allpassTuningL[i] + stereoSpread) * sampleRateRatio);

        allpassFiltersL[i].setBufferSize(delaySamplesL);
        allpassFiltersR[i].setBufferSize(delaySamplesR);
    }

    // Initialize tone shaper
    toneShaper.prepare(sampleRate, samplesPerBlock);

    // Initialize pre-delay buffer (max 100ms)
    int maxPreDelaySamples = static_cast<int>(0.1 * sampleRate) + 1;
    preDelayBuffer.setSize(2, maxPreDelaySamples);
    preDelayBuffer.clear();
    preDelayWriteIndex = 0;

    // Initialize smoothed values
    smoothedWet.reset(sampleRate, 0.02);  // 20ms smoothing
    smoothedDry.reset(sampleRate, 0.02);
    smoothedWidth.reset(sampleRate, 0.02);

    // Pre-allocate wet buffer (avoid allocation in audio thread)
    wetBuffer.setSize(2, samplesPerBlock, false, false, true);

    // Apply initial parameters
    updateCombFilters();
    updateAllpassFilters();
}

void ReverbProcessor::reset()
{
    for (auto& comb : combFiltersL) comb.clear();
    for (auto& comb : combFiltersR) comb.clear();
    for (auto& ap : allpassFiltersL) ap.clear();
    for (auto& ap : allpassFiltersR) ap.clear();

    toneShaper.reset();
    preDelayBuffer.clear();
    preDelayWriteIndex = 0;
}

void ReverbProcessor::setParameters(const Parameters& params)
{
    currentParams = params;

    // Update comb filter feedback and damping
    updateCombFilters();

    // Update tone shaper
    toneShaper.setTone(params.warmCold);

    // Update pre-delay (atomic for thread safety)
    int newPreDelay = static_cast<int>((params.preDelay / 1000.0) * currentSampleRate);
    newPreDelay = juce::jmin(newPreDelay, preDelayBuffer.getNumSamples() - 1);
    preDelaySamples.store(newPreDelay);

    // Update smoothed values
    float wet = params.wetLevel * scaleWet;
    float dry = params.dryLevel * scaleDry;

    smoothedWet.setTargetValue(wet);
    smoothedDry.setTargetValue(dry);
    smoothedWidth.setTargetValue(params.width);
}

void ReverbProcessor::updateCombFilters()
{
    float roomSize = (currentParams.roomSize * scaleRoom) + offsetRoom;
    float damp = currentParams.damping * scaleDamp;

    // In freeze mode, set maximum feedback and zero damping
    if (currentParams.freeze)
    {
        roomSize = 1.0f;
        damp = 0.0f;
    }

    for (int i = 0; i < 8; ++i)
    {
        combFiltersL[i].setFeedback(roomSize);
        combFiltersL[i].setDamp(damp);
        combFiltersR[i].setFeedback(roomSize);
        combFiltersR[i].setDamp(damp);
    }
}

void ReverbProcessor::updateAllpassFilters()
{
    // Allpass filters use fixed feedback (0.5)
    // No update needed as feedback is constexpr
}

void ReverbProcessor::process(juce::AudioBuffer<float>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    if (numChannels < 1)
        return;

    // Handle mono: process as dual-mono
    const bool isMono = (numChannels == 1);
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = isMono ? leftChannel : buffer.getWritePointer(1);

    // Use pre-allocated wet buffer (resize without allocation if within capacity)
    wetBuffer.setSize(2, numSamples, false, false, true);
    wetBuffer.clear();

    float* wetLeft = wetBuffer.getWritePointer(0);
    float* wetRight = wetBuffer.getWritePointer(1);

    // Load atomic pre-delay value once per block for consistency
    const int currentPreDelaySamples = juce::jmin(preDelaySamples.load(),
                                                   preDelayBuffer.getNumSamples() - 1);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float inputL = leftChannel[sample];
        float inputR = isMono ? inputL : rightChannel[sample];

        // Apply pre-delay
        float delayedL = inputL;
        float delayedR = inputR;

        if (currentPreDelaySamples > 0)
        {
            int readIndex = (preDelayWriteIndex - currentPreDelaySamples + preDelayBuffer.getNumSamples())
                            % preDelayBuffer.getNumSamples();

            delayedL = preDelayBuffer.getSample(0, readIndex);
            delayedR = preDelayBuffer.getSample(1, readIndex);

            preDelayBuffer.setSample(0, preDelayWriteIndex, inputL);
            preDelayBuffer.setSample(1, preDelayWriteIndex, inputR);

            preDelayWriteIndex = (preDelayWriteIndex + 1) % preDelayBuffer.getNumSamples();
        }

        // Sum to mono for reverb input
        float monoInput = (delayedL + delayedR) * 0.5f * fixedGain;

        // Process through parallel comb filters
        float combOutL = 0.0f;
        float combOutR = 0.0f;

        for (int i = 0; i < 8; ++i)
        {
            combOutL += combFiltersL[i].process(monoInput);
            combOutR += combFiltersR[i].process(monoInput);
        }

        // Process through serial allpass filters
        for (int i = 0; i < 4; ++i)
        {
            combOutL = allpassFiltersL[i].process(combOutL);
            combOutR = allpassFiltersR[i].process(combOutR);
        }

        wetLeft[sample] = combOutL;
        wetRight[sample] = combOutR;
    }

    // Apply tone shaping to wet signal
    toneShaper.process(wetBuffer);

    // Mix dry and wet signals with width control
    for (int sample = 0; sample < numSamples; ++sample)
    {
        float wet = smoothedWet.getNextValue();
        float dry = smoothedDry.getNextValue();
        float width = smoothedWidth.getNextValue();

        float wetL = wetBuffer.getSample(0, sample);
        float wetR = wetBuffer.getSample(1, sample);

        // Apply width (1.0 = full stereo, 0.0 = mono)
        float wetMono = (wetL + wetR) * 0.5f;
        float wetSide = (wetL - wetR) * 0.5f;

        float finalWetL = wetMono + wetSide * width;
        float finalWetR = wetMono - wetSide * width;

        // Final mix
        leftChannel[sample] = leftChannel[sample] * dry + finalWetL * wet;
        if (!isMono)
        {
            rightChannel[sample] = rightChannel[sample] * dry + finalWetR * wet;
        }
    }
}
