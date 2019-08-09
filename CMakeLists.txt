cmake_minimum_required(VERSION 3.14)

project(librouse
    VERSION 0.1.0
    DESCRIPTION "a library for interactive animation"
    LANGUAGES C)

enable_testing()


include(cmake/build_options.cmake)
include(cmake/warnings.cmake)
include(cmake/dependencies.cmake)


add_subdirectory(lib)

function(add_rouse_linked_executable target)
    add_executable("${target}" ${ARGN})
    target_compile_features("${target}" PRIVATE c_std_11)
    target_compile_options("${target}" PRIVATE ${rouse_warnings})
    target_link_libraries("${target}" PUBLIC rouse)
endfunction()


if(BUILD_TESTS)
    add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
