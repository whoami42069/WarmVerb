#pragma once

#include <JuceHeader.h>
#include <vector>

class AllpassFilter
{
public:
    AllpassFilter() = default;

    void setBufferSize(int size)
    {
        if (size > 0)
        {
            buffer.resize(static_cast<size_t>(size), 0.0f);
            bufferSize = size;
            bufferIndex = 0;
        }
    }

    float process(float input)
    {
        if (bufferSize == 0)
            return input;

        float bufferOut = buffer[static_cast<size_t>(bufferIndex)];
        float output = -input + bufferOut;

        buffer[static_cast<size_t>(bufferIndex)] = input + (bufferOut * feedback);

        bufferIndex = (bufferIndex + 1) % bufferSize;

        return output;
    }

    void clear()
    {
        std::fill(buffer.begin(), buffer.end(), 0.0f);
        bufferIndex = 0;
    }

private:
    std::vector<float> buffer;
    int bufferSize = 0;
    int bufferIndex = 0;
    static constexpr float feedback = 0.5f; // Fixed allpass feedback coefficient
};
