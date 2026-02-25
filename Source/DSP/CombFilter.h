#pragma once

#include <JuceHeader.h>
#include <vector>

class CombFilter
{
public:
    CombFilter() = default;

    void setBufferSize(int size)
    {
        if (size > 0)
        {
            buffer.resize(static_cast<size_t>(size), 0.0f);
            bufferSize = size;
            bufferIndex = 0;
        }
    }

    void setFeedback(float newFeedback)
    {
        feedback = juce::jlimit(0.0f, 1.0f, newFeedback);
    }

    void setDamp(float newDamp)
    {
        damp1 = juce::jlimit(0.0f, 1.0f, newDamp);
        damp2 = 1.0f - damp1;
    }

    float process(float input)
    {
        if (bufferSize == 0)
            return input;

        float output = buffer[static_cast<size_t>(bufferIndex)];

        // Lowpass filter in feedback loop
        filterStore = (output * damp2) + (filterStore * damp1);

        // Denormal protection
        if (std::abs(filterStore) < 1e-15f)
            filterStore = 0.0f;

        // Write to buffer with feedback
        buffer[static_cast<size_t>(bufferIndex)] = input + (filterStore * feedback);

        // Advance buffer index
        bufferIndex = (bufferIndex + 1) % bufferSize;

        return output;
    }

    void clear()
    {
        std::fill(buffer.begin(), buffer.end(), 0.0f);
        filterStore = 0.0f;
        bufferIndex = 0;
    }

private:
    std::vector<float> buffer;
    int bufferSize = 0;
    int bufferIndex = 0;
    float feedback = 0.0f;
    float damp1 = 0.0f;     // Damping coefficient
    float damp2 = 1.0f;     // 1 - damp1
    float filterStore = 0.0f; // Lowpass filter state
};
