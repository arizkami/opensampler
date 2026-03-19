# PluginConfig.cmake
# Plugin configuration settings for OpenSampler

# Plugin metadata
set(OPENSAMPLER_VERSION_MAJOR 1)
set(OPENSAMPLER_VERSION_MINOR 0)
set(OPENSAMPLER_VERSION_PATCH 0)
set(OPENSAMPLER_VERSION "${OPENSAMPLER_VERSION_MAJOR}.${OPENSAMPLER_VERSION_MINOR}.${OPENSAMPLER_VERSION_PATCH}")

# Plugin identifiers
set(OPENSAMPLER_UNIQUE_ID "OSmP")
set(OPENSAMPLER_MFR_ID "OSmp")
set(OPENSAMPLER_BUNDLE_DOMAIN "com")
set(OPENSAMPLER_BUNDLE_MFR "OpenSampler")

# Plugin capabilities
set(OPENSAMPLER_CHANNEL_IO "0-2")
set(OPENSAMPLER_DOES_MIDI_IN ON)
set(OPENSAMPLER_DOES_MIDI_OUT OFF)
set(OPENSAMPLER_DOES_MPE OFF)
set(OPENSAMPLER_DOES_STATE_CHUNKS ON)
set(OPENSAMPLER_HAS_UI ON)

# UI settings
set(OPENSAMPLER_WIDTH 800)
set(OPENSAMPLER_HEIGHT 600)
set(OPENSAMPLER_FPS 60)

# Plugin formats to build
option(OPENSAMPLER_BUILD_VST3 "Build VST3 plugin" ON)
option(OPENSAMPLER_BUILD_CLAP "Build CLAP plugin" ON)
option(OPENSAMPLER_BUILD_STANDALONE "Build standalone application" ON)
option(OPENSAMPLER_BUILD_AU "Build AudioUnit plugin (macOS only)" OFF)

# Feature flags
option(OPENSAMPLER_USE_FLUIDSYNTH "Enable FluidSynth support" OFF)
option(OPENSAMPLER_USE_SFIZZ "Enable Sfizz support" OFF)
option(OPENSAMPLER_USE_ZYNADDSUBFX "Enable ZynAddSubFX support" OFF)

# Development options
option(OPENSAMPLER_BUILD_TESTS "Build unit tests" OFF)
option(OPENSAMPLER_BUILD_EXAMPLES "Build example projects" OFF)
option(OPENSAMPLER_ENABLE_ASAN "Enable AddressSanitizer" OFF)

# Print configuration
message(STATUS "OpenSampler Configuration:")
message(STATUS "  Version: ${OPENSAMPLER_VERSION}")
message(STATUS "  Build VST3: ${OPENSAMPLER_BUILD_VST3}")
message(STATUS "  Build CLAP: ${OPENSAMPLER_BUILD_CLAP}")
message(STATUS "  Build Standalone: ${OPENSAMPLER_BUILD_STANDALONE}")
message(STATUS "  Build AU: ${OPENSAMPLER_BUILD_AU}")
message(STATUS "  FluidSynth: ${OPENSAMPLER_USE_FLUIDSYNTH}")
message(STATUS "  Sfizz: ${OPENSAMPLER_USE_SFIZZ}")
message(STATUS "  ZynAddSubFX: ${OPENSAMPLER_USE_ZYNADDSUBFX}")
