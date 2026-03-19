# FindFluidSynth.cmake
# Finds the FluidSynth library
#
# This will define the following variables:
#   FLUIDSYNTH_FOUND
#   FLUIDSYNTH_INCLUDE_DIRS
#   FLUIDSYNTH_LIBRARIES
#
# and the following imported targets:
#   FluidSynth::FluidSynth

find_path(FLUIDSYNTH_INCLUDE_DIR
    NAMES fluidsynth.h
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/External/fluidsynth/include
        /usr/include
        /usr/local/include
)

find_library(FLUIDSYNTH_LIBRARY
    NAMES fluidsynth libfluidsynth
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/External/fluidsynth/build
        /usr/lib
        /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FluidSynth
    REQUIRED_VARS FLUIDSYNTH_LIBRARY FLUIDSYNTH_INCLUDE_DIR
)

if(FLUIDSYNTH_FOUND)
    set(FLUIDSYNTH_LIBRARIES ${FLUIDSYNTH_LIBRARY})
    set(FLUIDSYNTH_INCLUDE_DIRS ${FLUIDSYNTH_INCLUDE_DIR})
    
    if(NOT TARGET FluidSynth::FluidSynth)
        add_library(FluidSynth::FluidSynth UNKNOWN IMPORTED)
        set_target_properties(FluidSynth::FluidSynth PROPERTIES
            IMPORTED_LOCATION "${FLUIDSYNTH_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${FLUIDSYNTH_INCLUDE_DIR}"
        )
    endif()
endif()

mark_as_advanced(FLUIDSYNTH_INCLUDE_DIR FLUIDSYNTH_LIBRARY)
