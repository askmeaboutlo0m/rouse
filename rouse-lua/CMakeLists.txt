cmake_minimum_required(VERSION 3.14)

project(librouse_lua
    VERSION 0.1.0
    DESCRIPTION "Lua extension for rouse"
    LANGUAGES C)


set(ROUSE_EXPORT_FILE "" CACHE STRING
    "Path to the Rouse.cmake file that's generated when building rouse.")

if(NOT ROUSE_EXPORT_FILE)
    message(FATAL_ERROR "ROUSE_EXPORT_FILE not specified. It must be the path "
                        "to the Rouse.cmake file generated when building the "
                        "main rouse library and placed in the build directory.")
endif()


include("${CMAKE_CURRENT_LIST_DIR}/../cmake/common.cmake")
include("${ROUSE_EXPORT_FILE}")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources.cmake")


add_subdirectory(lib)
add_subdirectory(app)


report_asan_targets()
report_clang_tidy_targets()


export(TARGETS lua rouse_lua lrouse
       NAMESPACE RouseLua:: FILE RouseLua.cmake)
