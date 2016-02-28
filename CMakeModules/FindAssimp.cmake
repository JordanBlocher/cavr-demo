# Find the native ASSIMP includes and library.
# Once done this will define
#
#  ASSIMP_INCLUDE_DIRS   - where to find ASSIMP_.h, etc.
#  ASSIMP_LIBRARIES      - List of libraries when using ASSIMP_.
#  ASSIMP_FOUND          - True if ASSIMP_ found.

find_path(ASSIMP_INCLUDE_DIR assimp/anim.h)

find_library(ASSIMP_LIBRARY NAMES assimp)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ASSIMP_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(ASSIMP  DEFAULT_MSG
                                  ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR)

mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY )

if(ASSIMP_FOUND)
    set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR})
    set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
endif()
