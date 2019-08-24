if(ROUSE_EMSCRIPTEN)
    message(STATUS "Not using clang-tidy because it doesn't "
                   "work properly with Emscripten")
else()
    if(USE_CLANG_TIDY AND NOT DEFINED CMAKE_C_CLANG_TIDY)
        find_program(clang_tidy NAMES "clang-tidy")
        if(clang_tidy)
            message(STATUS "Found clang-tidy, using it for all targets")
            set(ROUSE_CLANG_TIDY "${clang_tidy}")
        else()
            message(STATUS "Didn't find clang-tidy, so no linting for you")
        endif()
    else()
        message(STATUS "Not using clang-tidy as requested")
    endif()
endif()


function(add_clang_tidy_if_available target)
    if(ROUSE_CLANG_TIDY)
        set_target_properties("${target}" PROPERTIES
            C_CLANG_TIDY "${ROUSE_CLANG_TIDY}")
    endif()
endfunction()
