# WarmVerb - VST Reverb Plugin

A beginner-friendly algorithmic reverb VST plugin with warm/cold tonal control.

## Features

- **Freeverb-style Reverb** - Classic Schroeder-Moorer algorithm
- **Warm/Cold Character Control** - Shape the tonal character
- **5 Factory Presets** - Small Room, Medium Hall, Large Hall, Warm Plate, Shimmer
- **User Preset System** - Save and load your own presets
- **Pre-delay Control** - Up to 100ms
- **Freeze Mode** - Infinite sustain
- **Cross-platform** - Windows (VST3) and macOS (VST3 + AU)

## Parameters

| Parameter | Range | Description |
|-----------|-------|-------------|
| Room Size | 0-100% | Reverb decay time |
| Damping | 0-100% | High frequency absorption |
| Width | 0-100% | Stereo spread |
| Wet | 0-100% | Reverb signal level |
| Dry | 0-100% | Direct signal level |
| Character | Cold to Warm | Tonal control |
| Pre-Delay | 0-100ms | Initial reflection delay |
| Freeze | On/Off | Infinite sustain |

## Build Instructions

### Prerequisites

- **CMake** 3.22 or later
- **C++ Compiler** with C++17 support
  - Windows: Visual Studio 2019/2022
  - macOS: Xcode 12+
- **Git** (to fetch JUCE automatically)

### Windows Build

```powershell
# Open PowerShell/Command Prompt in project directory
cd C:\Users\hp\Desktop\talhasinan\WarmVerb

# Generate Visual Studio solution
cmake -B build -G "Visual Studio 17 2022"

# Build Release version
cmake --build build --config Release

# Plugin will be at:
# build\WarmVerb_artefacts\Release\VST3\WarmVerb.vst3
```

### macOS Build

```bash
# Open Terminal in project directory
cd ~/Desktop/talhasinan/WarmVerb

# Generate Xcode project
cmake -B build -G Xcode

# Build Release version
cmake --build build --config Release

# Plugins will be at:
# build/WarmVerb_artefacts/Release/VST3/WarmVerb.vst3
# build/WarmVerb_artefacts/Release/AU/WarmVerb.component
```

## Installation

### Windows
Copy `WarmVerb.vst3` to:
- `C:\Program Files\Common Files\VST3\`

### macOS
Copy plugins to:
- VST3: `~/Library/Audio/Plug-Ins/VST3/` or `/Library/Audio/Plug-Ins/VST3/`
- AU: `~/Library/Audio/Plug-Ins/Components/` or `/Library/Audio/Plug-Ins/Components/`

## Project Structure

```
WarmVerb/
├── CMakeLists.txt          # Build configuration
├── Source/
│   ├── PluginProcessor.*   # Audio processing
│   ├── PluginEditor.*      # GUI
│   ├── DSP/                # DSP algorithms
│   │   ├── CombFilter.*    # Comb filter
│   │   ├── AllpassFilter.* # Allpass filter
│   │   ├── ReverbProcessor.* # Main reverb
│   │   └── ToneShaper.*    # Warm/Cold EQ
│   ├── GUI/                # UI components
│   │   ├── CustomLookAndFeel.* # Visual styling
│   │   ├── ReverbKnob.*    # Knob component
│   │   └── PresetBar.*     # Preset selector
│   ├── Presets/            # Preset system
│   │   ├── PresetManager.* # Save/load presets
│   │   └── FactoryPresets.h # Built-in presets
│   └── Utils/              # Utilities
│       ├── ParameterIDs.h  # Parameter constants
│       └── VersionInfo.h   # Version info
├── Resources/              # Assets
└── Installers/             # Installer scripts
```

## License

MIT License

## Version

1.0.0
