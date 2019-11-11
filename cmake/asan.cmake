if(ROUSE_EMSCRIPTEN)
    message(STATUS "Not using address sanitizer because this is Emscripten")
elseif(USE_ASAN)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(rouse_asan_options -fno-omit-frame-pointer -fsanitize=address)
        message(STATUS "Using address sanitizer for gcc/clang "
                        "via options '${rouse_asan_options}'")
    else()
        message(STATUS "Don't know how to enable address "
                        "sanitizer on ${CMAKE_C_COMPILER_ID}")
    endif()
else()
    message(STATUS "Not using address sanitizer as requested")
endif()


function(add_asan_if_available target)
    if(rouse_asan_options)
        set_property(GLOBAL APPEND PROPERTY rouse_asan_targets "${target}")
        target_compile_definitions("${target}" PRIVATE "ROUSE_ASAN")
        target_compile_options("${target}" PRIVATE ${rouse_asan_options})
        target_link_options("${target}" PRIVATE ${rouse_asan_options})
    endif()
endfunction()

function(report_asan_targets)
    get_property(targets GLOBAL PROPERTY rouse_asan_targets)
    message(STATUS "Targets with address sanitizer: ${targets}")
endfunction()
