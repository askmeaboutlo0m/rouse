cmake_minimum_required(VERSION 3.14)

project(librouse
    VERSION 0.1.0
    DESCRIPTION "a library for interactive animation"
    LANGUAGES C)

enable_testing()


include(cmake/build_options.cmake)
include(cmake/warnings.cmake)
include(cmake/asan.cmake)
include(cmake/clang_tidy.cmake)
include(cmake/dependencies.cmake)
include(cmake/rouse_config.cmake)
include(cmake/sources.cmake)


function(set_c11_standard target)
    set_target_properties("${target}" PROPERTIES
            C_STANDARD          11
            C_STANDARD_REQUIRED ON
            C_EXTENSIONS        OFF)
endfunction()


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


export(TARGETS rouse nanovg parson cglm sdl2 sdl2_image
       NAMESPACE Rouse:: FILE Rouse.cmake)
