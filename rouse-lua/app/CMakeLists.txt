add_library(lrouse STATIC "${CMAKE_CURRENT_LIST_DIR}/lrouse.c")
set_c11_standard(lrouse)
target_compile_options(lrouse PRIVATE ${rouse_warnings})
target_link_libraries(lrouse PUBLIC rouse_lua)
add_asan_if_available(lrouse)
add_clang_tidy_if_available(lrouse)

# On Emscripten you specify all your assets and scripts when building the
# executable, so building one without any scripts in it won't be useful.
if(NOT ROUSE_EMSCRIPTEN)
    add_executable(lrouse_exe "${CMAKE_CURRENT_LIST_DIR}/main.c")
    set_c11_standard(lrouse_exe)
    target_compile_options(lrouse_exe PRIVATE ${rouse_warnings})
    target_link_libraries(lrouse_exe PUBLIC lrouse)
    add_asan_if_available(lrouse_exe)
    add_clang_tidy_if_available(lrouse_exe)
    set_target_properties(lrouse_exe PROPERTIES OUTPUT_NAME "lrouse")
endif()
