
project(common)

file(GLOB_RECURSE SRCS "src/*.cpp")

add_library(common ${SRCS} )

TARGET_LINK_LIBRARIES(common cavr cavrgl cavrgfx IrrKlang ikpMP3)
