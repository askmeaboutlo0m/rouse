cmake_minimum_required(VERSION 3.14)

project(librouse
    VERSION 0.2.0
    DESCRIPTION "a library for interactive animation"
    LANGUAGES C)

enable_testing()


include("${CMAKE_CURRENT_LIST_DIR}/../cmake/common.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/build_options.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/dependencies.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/rouse_config.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources.cmake")


add_subdirectory(lib)

function(add_rouse_linked_executable target)
    add_executable("${target}" ${ARGN})
    add_asan_if_available("${target}")
    add_clang_tidy_if_available("${target}")
    set_c11_standard("${target}")
    target_compile_options("${target}" PRIVATE ${rouse_warnings})
    target_link_libraries("${target}" PUBLIC rouse)
endfunction()


if(BUILD_TESTS)
    add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


report_asan_targets()
report_clang_tidy_targets()


set(rouse_exports rouse nanovg parson cglm sdl2 sdl2_image)

if(BUILD_WITH_OPENAL)
    list(APPEND rouse_exports openal stb_vorbis)
endif()

export(TARGETS ${rouse_exports} NAMESPACE Rouse:: FILE Rouse.cmake)
