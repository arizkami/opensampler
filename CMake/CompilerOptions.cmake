# CompilerOptions.cmake
# Compiler-specific options for OpenSampler

if(MSVC)
    # MSVC specific options
    add_compile_options(
        /W4                     # Warning level 4
        /WX-                    # Don't treat warnings as errors
        /permissive-           # Standards conformance mode
        /Zc:__cplusplus        # Enable updated __cplusplus macro
        /MP                     # Multi-processor compilation
    )
    
    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        _UNICODE
        UNICODE
    )
    
    # Release optimizations
    add_compile_options($<$<CONFIG:Release>:/O2 /Ob2 /Oi /Ot /GL>)
    add_link_options($<$<CONFIG:Release>:/LTCG /OPT:REF /OPT:ICF>)
    
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC/Clang options
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
        -fvisibility=hidden
    )
    
    # Release optimizations
    add_compile_options($<$<CONFIG:Release>:-O3 -march=native>)
    
    if(APPLE)
        add_compile_options(-mmacosx-version-min=10.13)
    endif()
endif()

# Enable position independent code for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Debug/Release configurations
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
