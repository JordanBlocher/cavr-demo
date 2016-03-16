project(common)
FIND_PACKAGE(Freetype)
INCLUDE_DIRECTORIES(
  "${FREETYPE_INCLUDE_DIR_freetype2}"
)


file(GLOB_RECURSE SRCS "src/*.cpp")

add_library(common ${SRCS} )

TARGET_LINK_LIBRARIES(common cavr cavrgl cavrgfx IrrKlang ikpMP3 ${FREETYPE_LIBRARIES} )
