cmake_minimum_required(VERSION 3.22)
project(WarmVerb VERSION 1.0.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch JUCE if not present locally
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/CMakeLists.txt")
    add_subdirectory(JUCE)
else()
    include(FetchContent)
    FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG        7.0.9
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(JUCE)
endif()

# Define the plugin target
juce_add_plugin(WarmVerb
    COMPANY_NAME "WarmVerb Audio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Wvba
    PLUGIN_CODE Wvrb
    FORMATS VST3 AU Standalone
    VST3_CATEGORIES "Fx" "Reverb"
    AU_MAIN_TYPE "kAudioUnitType_Effect"
    PRODUCT_NAME "WarmVerb"
)

# Add source files
target_sources(WarmVerb PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/DSP/CombFilter.cpp
    Source/DSP/AllpassFilter.cpp
    Source/DSP/ReverbProcessor.cpp
    Source/DSP/ToneShaper.cpp
    Source/GUI/CustomLookAndFeel.cpp
    Source/GUI/ReverbKnob.cpp
    Source/GUI/PresetBar.cpp
    Source/Presets/PresetManager.cpp
)

# Define preprocessor definitions
target_compile_definitions(WarmVerb PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

# Include directories
target_include_directories(WarmVerb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Link JUCE modules
target_link_libraries(WarmVerb PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
