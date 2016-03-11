PROJECT(Example)


FIND_PACKAGE(Lua51 REQUIRED)
FIND_PACKAGE(GLOG REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
#FIND_PACKAGE(CAVR REQUIRED)
FIND_PACKAGE(ImageMagick REQUIRED)
FIND_PACKAGE(Bullet REQUIRED)
FIND_PACKAGE(Assimp REQUIRED)
#FIND_PACKAGE(Magick++ REQUIRED)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
SET(CXX11_FLAGS -std=gnu++11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")

IF(UNIX)
	ADD_DEFINITIONS(-DUNIX)
ENDIF(UNIX)


INCLUDE_DIRECTORIES(
	"${LUA_INCLUDE_DIR}"
	"${CAVR_INCLUDE_PATH}"
	"${HOME_BUILD_PATH}/IrrKlang/include"
  	"${PROJECT_SOURCE_DIR}/include"
  	"${PROJECT_SOURCE_DIR}/common/include"
	"${GLOG_INCLUDE_DIRS}"
    "${ASSIMP_INCLUDE_DIR}"
    #"${IMAGEMAGIC_INCLUDE_DIRS}"
    "${BULLET_INCLUDE_DIRS}"
    #"${MAGIC++_INCLUDE_DIRS}"  

    "/usr/include/ImageMagick"
)

LINK_DIRECTORIES(
	"${HOME_BUILD_PATH}/IrrKlang/lib"
	"${CAVR_LIBRARIES}"
)

ADD_SUBDIRECTORY(common)

# Set Includes
SET(INCLUDES ${PROJECT_SOURCE_DIR}/include)
#MESSAGE( STATUS "FILES: " ${INCLUDES} )
INCLUDE_DIRECTORIES(${INCLUDES})

# Set sources
file(GLOB_RECURSE SOURCES "source/*.cpp")
ADD_EXECUTABLE(Example ${SOURCES})

add_custom_target(BUILD_${PROJECT_NAME} ALL
                  DEPENDS Example
                  COMMAND ${CMAKE_COMMAND} -E echo ""
                  COMMAND ${CMAKE_COMMAND} -E echo "====================="
                  COMMAND ${CMAKE_COMMAND} -E echo "  Compile complete!"
                  COMMAND ${CMAKE_COMMAND} -E echo "====================="
                  COMMAND ${CMAKE_COMMAND} -E echo "${CMAKE_CURRENT_BINARY_DIR}"
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
		  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/media/ ${CMAKE_CURRENT_BINARY_DIR}/media
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/config/ ${CMAKE_CURRENT_BINARY_DIR}/config
                  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/config/cavrconfig.lua ${CMAKE_CURRENT_BINARY_DIR}
                  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/config/cavrplugins.lua ${CMAKE_CURRENT_BINARY_DIR}

                 )

             TARGET_LINK_LIBRARIES(Example cavr cavrgl cavrgfx IrrKlang common pthread Magick++ ${BULLET_LIBRARIES} ${MAGIC++_LIBRARY} ${IMAGEMAGIC_LIBRARIES} ${ASSIMP_LIBRARY} ${GLOG_LIBRARIES} ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${CAVR_LIBRARIES} )
