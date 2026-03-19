cmake_minimum_required(VERSION 3.15)

project(OpenSampler VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IPLUG2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/External/iPlug2")

add_subdirectory(${IPLUG2_ROOT} iPlug2)

set(PLUG_NAME "OpenSampler")
set(PLUG_MFR "OpenSampler")
set(PLUG_VERSION_HEX "0x00010000")
set(PLUG_VERSION_STR "1.0.0")
set(PLUG_UNIQUE_ID "OSmP")
set(PLUG_MFR_ID "OSmp")
set(PLUG_URL_STR "https://github.com/opensampler")
set(PLUG_EMAIL_STR "info@opensampler.com")
set(PLUG_COPYRIGHT_STR "Copyright 2026")
set(PLUG_CLASS_NAME "OpenSampler")

set(BUNDLE_NAME "OpenSampler")
set(BUNDLE_MFR "OpenSampler")
set(BUNDLE_DOMAIN "com")

set(PLUG_CHANNEL_IO "0-2")

set(PLUG_TYPE "1")
set(PLUG_DOES_MIDI_IN "1")
set(PLUG_DOES_MIDI_OUT "0")
set(PLUG_DOES_MPE "0")
set(PLUG_DOES_STATE_CHUNKS "1")
set(PLUG_HAS_UI "1")

set(PLUG_WIDTH "800")
set(PLUG_HEIGHT "600")
set(PLUG_FPS "60")

set(VST3_SUBCATEGORY "Instrument|Sampler")

set(OPENSAMPLER_SOURCES
    Application.cpp
    Core/OSMPEngine.cpp
    Graphic/OSMPGui.cpp
)

set(OPENSAMPLER_HEADERS
    config.h
    include/Core/OSMPEngine.hpp
    include/Graphic/OSMPGui.hpp
)

add_library(OpenSampler_Core STATIC
    ${OPENSAMPLER_SOURCES}
    ${OPENSAMPLER_HEADERS}
)

target_include_directories(OpenSampler_Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${IPLUG2_ROOT}/IPlug
    ${IPLUG2_ROOT}/IGraphics
    ${IPLUG2_ROOT}/Dependencies/IGraphics/build/src
    ${IPLUG2_ROOT}/Dependencies/IPlug/VST3_SDK
    ${IPLUG2_ROOT}/WDL
)

target_compile_definitions(OpenSampler_Core PUBLIC
    IPLUG_DSP=1
    IPLUG_EDITOR=1
    VST3_API=1
    CLAP_API=1
    APP_API=1
)

if(MSVC)
    target_compile_options(OpenSampler_Core PRIVATE /W4)
    target_compile_definitions(OpenSampler_Core PRIVATE
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
    )
else()
    target_compile_options(OpenSampler_Core PRIVATE -Wall -Wextra -Wpedantic)
endif()

function(create_plugin_target TARGET_NAME PLUGIN_TYPE)
    add_executable(${TARGET_NAME}
        ${OPENSAMPLER_SOURCES}
        ${OPENSAMPLER_HEADERS}
    )
    
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${IPLUG2_ROOT}/IPlug
        ${IPLUG2_ROOT}/IGraphics
        ${IPLUG2_ROOT}/Dependencies/IGraphics/build/src
        ${IPLUG2_ROOT}/Dependencies/IPlug/VST3_SDK
        ${IPLUG2_ROOT}/WDL
    )
    
    target_compile_definitions(${TARGET_NAME} PRIVATE
        IPLUG_DSP=1
        IPLUG_EDITOR=1
        ${PLUGIN_TYPE}=1
    )
    
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /W4)
        target_compile_definitions(${TARGET_NAME} PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
        )
    endif()
    
    target_link_libraries(${TARGET_NAME} PRIVATE
        iPlug2_IPlug
        iPlug2_IGraphics
    )
endfunction()

create_plugin_target(OpenSampler_VST3 VST3_API)
create_plugin_target(OpenSampler_CLAP CLAP_API)
create_plugin_target(OpenSampler_APP APP_API)

set_target_properties(OpenSampler_VST3 PROPERTIES
    OUTPUT_NAME "OpenSampler"
    SUFFIX ".vst3"
)

set_target_properties(OpenSampler_CLAP PROPERTIES
    OUTPUT_NAME "OpenSampler"
    SUFFIX ".clap"
)

set_target_properties(OpenSampler_APP PROPERTIES
    OUTPUT_NAME "OpenSampler"
)

install(TARGETS OpenSampler_VST3 OpenSampler_CLAP OpenSampler_APP
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

message(STATUS "OpenSampler Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  iPlug2 Root: ${IPLUG2_ROOT}")
message(STATUS "  Build Types: VST3, CLAP, Standalone")
message(STATUS "  Plugin Type: Instrument/Sampler")
