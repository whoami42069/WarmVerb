#pragma once

#include <JuceHeader.h>
#include "DSP/ReverbProcessor.h"

class WarmVerbAudioProcessor : public juce::AudioProcessor
{
public:
    WarmVerbAudioProcessor();
    ~WarmVerbAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::AudioProcessorValueTreeState apvts;
    ReverbProcessor reverbProcessor;

    // Atomic parameters for thread-safe access
    std::atomic<float>* roomSizeParam = nullptr;
    std::atomic<float>* dampingParam = nullptr;
    std::atomic<float>* widthParam = nullptr;
    std::atomic<float>* wetLevelParam = nullptr;
    std::atomic<float>* dryLevelParam = nullptr;
    std::atomic<float>* warmColdParam = nullptr;
    std::atomic<float>* preDelayParam = nullptr;
    std::atomic<float>* freezeParam = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WarmVerbAudioProcessor)
};
