# FindSfizz.cmake
# Finds the Sfizz library
#
# This will define the following variables:
#   SFIZZ_FOUND
#   SFIZZ_INCLUDE_DIRS
#   SFIZZ_LIBRARIES
#
# and the following imported targets:
#   Sfizz::Sfizz

find_path(SFIZZ_INCLUDE_DIR
    NAMES sfizz.h sfizz.hpp
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/External/sfizz/src
        ${CMAKE_CURRENT_SOURCE_DIR}/External/sfizz/include
        /usr/include
        /usr/local/include
)

find_library(SFIZZ_LIBRARY
    NAMES sfizz libsfizz
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/External/sfizz/build
        /usr/lib
        /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sfizz
    REQUIRED_VARS SFIZZ_LIBRARY SFIZZ_INCLUDE_DIR
)

if(SFIZZ_FOUND)
    set(SFIZZ_LIBRARIES ${SFIZZ_LIBRARY})
    set(SFIZZ_INCLUDE_DIRS ${SFIZZ_INCLUDE_DIR})
    
    if(NOT TARGET Sfizz::Sfizz)
        add_library(Sfizz::Sfizz UNKNOWN IMPORTED)
        set_target_properties(Sfizz::Sfizz PROPERTIES
            IMPORTED_LOCATION "${SFIZZ_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${SFIZZ_INCLUDE_DIR}"
        )
    endif()
endif()

mark_as_advanced(SFIZZ_INCLUDE_DIR SFIZZ_LIBRARY)
