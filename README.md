# OpenSampler

A modern, open-source sampler plugin built with iPlug2 framework.

## Overview

OpenSampler is a professional-grade audio sampler instrument plugin supporting multiple formats including VST3, CLAP, and Standalone applications. Built with modern C++17 and leveraging the powerful iPlug2 framework for cross-platform compatibility.

## Features

- **Multi-format Support**: VST3, CLAP, and Standalone (APP)
- **Polyphonic Playback**: Up to 32 simultaneous voices with intelligent voice stealing
- **ADSR Envelope**: Full Attack, Decay, Sustain, Release controls
- **Sample Management**: Load and manage multiple samples with velocity and key mapping
- **Modern UI**: Clean, responsive interface built with Inter font family
- **MIDI Support**: Full MIDI input handling with note on/off and control messages
- **Cross-platform**: Windows, macOS, and Linux support

## Project Structure

```
OpenSampler/
├── Application.cpp              # Main plugin entry point
├── config.h                     # Plugin configuration and metadata
├── CMakeLists.txt              # Main CMake build configuration
├── CMake/                      # CMake modules and find scripts
│   ├── CompilerOptions.cmake   # Compiler-specific settings
│   ├── PluginConfig.cmake      # Plugin build options
│   ├── FindFluidSynth.cmake    # FluidSynth finder
│   └── FindSfizz.cmake         # Sfizz finder
├── Core/                       # Audio engine implementation
│   └── OSMPEngine.cpp
├── Graphic/                    # GUI implementation
│   ├── OSMPGui.cpp
│   └── Resources/
│       ├── Fonts/Inter/        # Inter font family
│       ├── Vectors/Lucide/     # Lucide icon set
│       ├── Images/             # Image resources
│       ├── Samples/            # Demo samples
│       └── Presets/            # Factory presets
├── include/                    # Header files
│   ├── Core/
│   │   └── OSMPEngine.hpp
│   └── Graphic/
│       └── OSMPGui.hpp
├── Modules/                    # Additional plugin modules
├── External/                   # External dependencies
│   ├── iPlug2/                 # iPlug2 framework
│   ├── fluidsynth/             # FluidSynth library
│   ├── sfizz/                  # Sfizz SFZ player
│   ├── zynaddsubfx/            # ZynAddSubFX synthesizer
│   └── NeuralAmpModelerCore/   # Neural Amp Modeler
├── Tests/                      # Unit tests
├── Docs/                       # Documentation
└── Build/                      # Build output directory

```

## Building

### Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler (MSVC 2019+, GCC 9+, Clang 10+)
- Git with submodules initialized

### Build Instructions

#### Windows (Visual Studio)

```bash
cd OpenSampler
cmake -B Build -G "Visual Studio 17 2022"
cmake --build Build --config Release
```

#### macOS / Linux

```bash
cd OpenSampler
cmake -B Build -DCMAKE_BUILD_TYPE=Release
cmake --build Build
```

### Build Options

Configure build options using CMake:

```bash
cmake -B Build \
  -DOPENSAMPLER_BUILD_VST3=ON \
  -DOPENSAMPLER_BUILD_CLAP=ON \
  -DOPENSAMPLER_BUILD_STANDALONE=ON \
  -DOPENSAMPLER_USE_FLUIDSYNTH=OFF \
  -DOPENSAMPLER_USE_SFIZZ=OFF
```

Available options:
- `OPENSAMPLER_BUILD_VST3` - Build VST3 plugin (default: ON)
- `OPENSAMPLER_BUILD_CLAP` - Build CLAP plugin (default: ON)
- `OPENSAMPLER_BUILD_STANDALONE` - Build standalone app (default: ON)
- `OPENSAMPLER_BUILD_AU` - Build AudioUnit (macOS only, default: OFF)
- `OPENSAMPLER_USE_FLUIDSYNTH` - Enable FluidSynth support (default: OFF)
- `OPENSAMPLER_USE_SFIZZ` - Enable Sfizz SFZ support (default: OFF)
- `OPENSAMPLER_BUILD_TESTS` - Build unit tests (default: OFF)

## Development

### Architecture

**Core Engine (`OSMPEngine`)**
- Sample management and loading
- Voice allocation and polyphony
- ADSR envelope processing
- Audio rendering and mixing

**GUI (`OSMPGui`)**
- Parameter controls (knobs, sliders)
- Visual feedback and metering
- Sample browser and management
- Preset management

### Adding Features

1. Implement core functionality in `Core/OSMPEngine.cpp`
2. Add GUI controls in `Graphic/OSMPGui.cpp`
3. Wire parameters in `Application.cpp`
4. Update `config.h` if needed

## Dependencies

- **iPlug2**: Plugin framework (ZLib License)
- **FluidSynth**: SoundFont synthesizer (LGPL)
- **Sfizz**: SFZ format player (BSD-2)
- **ZynAddSubFX**: Software synthesizer (GPL)
- **Inter Font**: UI typography (OFL)
- **Lucide Icons**: Icon set (ISC)

## License

This Project is MIT License

## Contributing

Contributions are welcome! Please read our contributing guidelines before submitting pull requests.

## Contact

- Website: https://github.com/arizkami/opensampler
- Email: info@opensampler.app

## Acknowledgments

- iPlug2 framework by Oli Larkin and Alex Harker
- Inter typeface by Rasmus Andersson
- Lucide icon set
