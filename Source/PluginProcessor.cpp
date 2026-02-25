#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils/ParameterIDs.h"

WarmVerbAudioProcessor::WarmVerbAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // Cache parameter pointers for thread-safe access
    roomSizeParam = apvts.getRawParameterValue(ParamID::roomSize);
    dampingParam = apvts.getRawParameterValue(ParamID::damping);
    widthParam = apvts.getRawParameterValue(ParamID::width);
    wetLevelParam = apvts.getRawParameterValue(ParamID::wetLevel);
    dryLevelParam = apvts.getRawParameterValue(ParamID::dryLevel);
    warmColdParam = apvts.getRawParameterValue(ParamID::warmCold);
    preDelayParam = apvts.getRawParameterValue(ParamID::preDelay);
    freezeParam = apvts.getRawParameterValue(ParamID::freeze);
}

WarmVerbAudioProcessor::~WarmVerbAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout WarmVerbAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParamID::roomSize, 1),
        "Room Size",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        ParameterDefaults::roomSize));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParamID::damping, 1),
        "Damping",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        ParameterDefaults::damping));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParamID::width, 1),
        "Width",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        ParameterDefaults::width));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParamID::wetLevel, 1),
        "Wet",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        ParameterDefaults::wetLevel));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParamID::dryLevel, 1),
        "Dry",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        ParameterDefaults::dryLevel));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParamID::warmCold, 1),
        "Character",
        juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f),
        ParameterDefaults::warmCold));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParamID::preDelay, 1),
        "Pre-Delay",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        ParameterDefaults::preDelay,
        juce::AudioParameterFloatAttributes().withLabel("ms")));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID(ParamID::freeze, 1),
        "Freeze",
        ParameterDefaults::freeze));

    return { params.begin(), params.end() };
}

const juce::String WarmVerbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WarmVerbAudioProcessor::acceptsMidi() const
{
    return false;
}

bool WarmVerbAudioProcessor::producesMidi() const
{
    return false;
}

bool WarmVerbAudioProcessor::isMidiEffect() const
{
    return false;
}

double WarmVerbAudioProcessor::getTailLengthSeconds() const
{
    return 3.0; // Reverb tail can be long
}

int WarmVerbAudioProcessor::getNumPrograms()
{
    return 1;
}

int WarmVerbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WarmVerbAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String WarmVerbAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void WarmVerbAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void WarmVerbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    reverbProcessor.prepare(sampleRate, samplesPerBlock);
}

void WarmVerbAudioProcessor::releaseResources()
{
    reverbProcessor.reset();
}

bool WarmVerbAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Support mono and stereo
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // Input must match output
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void WarmVerbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                          juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Null check for parameter pointers (safety)
    if (roomSizeParam == nullptr || dampingParam == nullptr || widthParam == nullptr ||
        wetLevelParam == nullptr || dryLevelParam == nullptr || warmColdParam == nullptr ||
        preDelayParam == nullptr || freezeParam == nullptr)
        return;

    // Update reverb parameters
    ReverbProcessor::Parameters params;
    params.roomSize = roomSizeParam->load();
    params.damping = dampingParam->load();
    params.width = widthParam->load();
    params.wetLevel = wetLevelParam->load();
    params.dryLevel = dryLevelParam->load();
    params.warmCold = warmColdParam->load();
    params.preDelay = preDelayParam->load();
    params.freeze = freezeParam->load() > 0.5f;

    reverbProcessor.setParameters(params);

    // Process audio
    reverbProcessor.process(buffer);
}

bool WarmVerbAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* WarmVerbAudioProcessor::createEditor()
{
    return new WarmVerbAudioProcessorEditor(*this);
}

void WarmVerbAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void WarmVerbAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(apvts.state.getType()))
        {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WarmVerbAudioProcessor();
}
