
project(common)

file(GLOB_RECURSE SRCS "src/*.cpp")

add_library(common ${SRCS} )
