function(add_lrouse_executable exe questionable_features)
    add_executable(${exe} "${CMAKE_CURRENT_LIST_DIR}/lrouse.c")
    set_language_standards(${exe})
    target_compile_options(${exe} PRIVATE ${rouse_cflags})
    target_link_libraries(${exe} PUBLIC rouse_lua)
    add_asan_if_available(${exe})
    add_clang_tidy_if_available(${exe})
    set_target_properties(${exe} PROPERTIES OUTPUT_NAME "${exe}")

    target_compile_definitions(${exe} PRIVATE
        "ROUSE_LUA_QUESTIONABLE_FEATURES=${questionable_features}")

    if(BUILD_WITH_STATIC_CXXLIBS AND BUILD_WITH_BOX2D)
        target_link_libraries(${exe} PRIVATE "-static-libgcc" "-static-libstdc++")
    endif()

    if(ROUSE_EMSCRIPTEN)
        target_link_options(${exe} PRIVATE "-sFETCH" "-sALLOW_MEMORY_GROWTH=1")
        set_property(TARGET ${exe} PROPERTY SUFFIX ".html")
        if(ROUSE_EM_SHELL)
            target_link_options(${exe} PRIVATE "--shell-file=${ROUSE_EM_SHELL}")
        endif()
    endif()
endfunction()

add_lrouse_executable(lrouse 0)
add_lrouse_executable(lrouse-with-questionable-features 1)
