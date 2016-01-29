INCLUDE(FindPackageHandleStandardArgs)

FIND_PATH(CAVR_INCLUDE_PATH cavr/system.h)
FIND_LIBRARY(CAVR_LIBRARY_PATH cavr)
FIND_LIBRARY(CAVR_PLUGIN_PATH PATHS "$ENV{CAVR_PLUGIN_PATH}" "/usr/local/cavr/plugins" *.so}
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CAVR DEFAULT_MSG 
                                  CAVR_INCLUDE_PATH 
                                  CAVR_LIBRARY_PATH
				  CAVR_PLUGIN_PATH)

IF(CAVR_FOUND)
  SET(CAVR_INCLUDE_DIRS ${CAVR_INCLUDE_PATH})
  SET(CAVR_LIBRARIES ${CAVR_LIBRARY_PATH} ${CAVR_PLUGIN_PATH})
ENDIF(CAVR_FOUND)

