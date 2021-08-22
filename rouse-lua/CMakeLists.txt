cmake_minimum_required(VERSION 3.14)

project(librouse_lua
    VERSION 0.2.0
    DESCRIPTION "Lua extension for rouse"
    LANGUAGES C CXX)


set(ROUSE_EXPORT_FILE "" CACHE STRING
    "Path to the Rouse.cmake file that's generated when building rouse.")

if(NOT ROUSE_EXPORT_FILE)
    message(FATAL_ERROR "ROUSE_EXPORT_FILE not specified. It must be the path "
                        "to the Rouse.cmake file generated when building the "
                        "main rouse library and placed in the build directory.")
endif()


include("${CMAKE_CURRENT_LIST_DIR}/../cmake/common.cmake")
include("${ROUSE_EXPORT_FILE}")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/build_options.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources.cmake")


add_subdirectory(lib)
add_subdirectory(app)


report_asan_targets()
report_clang_tidy_targets()


set(rouse_lua_export_targets lua rouse_lua lrouse)

if(BUILD_WITH_BOX2D)
    list(APPEND rouse_lua_export_targets rouse_lua_box2d)
endif()

if(BUILD_WITH_IMGUI)
    list(APPEND rouse_lua_export_targets imgui imnodes rouse_lua_imgui )
endif()

export(TARGETS ${rouse_lua_export_targets}
       NAMESPACE RouseLua:: FILE RouseLua.cmake)
