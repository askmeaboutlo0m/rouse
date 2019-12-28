if(CMAKE_BUILD_TYPE MATCHES "Rel")
    set(ROUSE_DEBUG OFF)
else()
    set(ROUSE_DEBUG ON)
endif()

if(BUILD_WITH_MAGIC)
    set(ROUSE_MAGIC ON)
else()
    set(ROUSE_MAGIC OFF)
endif()

if(BUILD_WITH_GL_CHECKS)
    set(ROUSE_GL_CHECKS ON)
else()
    set(ROUSE_GL_CHECKS OFF)
endif()

message(STATUS "ROUSE_DEBUG is ${ROUSE_DEBUG}")
message(STATUS "ROUSE_MAGIC is ${ROUSE_MAGIC}")
message(STATUS "ROUSE_GL_CHECKS is ${ROUSE_GL_CHECKS}")

set(rouse_config_h "${CMAKE_BINARY_DIR}/config/rouse_config.h")
configure_file(cmake/rouse_config.tpl.h "${rouse_config_h}")
