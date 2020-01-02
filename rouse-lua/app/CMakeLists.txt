add_executable(lrouse "${CMAKE_CURRENT_LIST_DIR}/lrouse.c")
set_c11_standard(lrouse)
target_compile_options(lrouse PRIVATE ${rouse_warnings})
target_link_libraries(lrouse PUBLIC rouse_lua)
add_asan_if_available(lrouse)
add_clang_tidy_if_available(lrouse)
set_target_properties(lrouse PROPERTIES OUTPUT_NAME "lrouse")

if(ROUSE_EMSCRIPTEN)
    target_compile_options(lrouse PRIVATE "-sFETCH=1")
    target_link_options(   lrouse PRIVATE "-sFETCH=1"
                                          "-sALLOW_MEMORY_GROWTH=1")
    set_property(TARGET lrouse PROPERTY SUFFIX ".html")
endif()
