add_library(lua STATIC "${lua_sources}" "${lua_headers}")
set_target_properties(lua PROPERTIES OUTPUT_NAME rouse_lua_5_3_5)
set_language_standards(lua)
add_asan_if_available(lua)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(lua PUBLIC LUA_USE_APICHECK)
endif()


function(process_xls inputs_name suffix outputs_name)
    unset(outputs)
    foreach(input_xl IN LISTS "${inputs_name}")
        string(REGEX REPLACE "\\.xl$" "${suffix}" output_c "${input_xl}")
        add_custom_command(OUTPUT "${output_c}"
            COMMAND "${PROJECT_SOURCE_DIR}/xlua/xl2c"
            ARGS "${input_xl}" "${output_c}"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            DEPENDS "${input_xl}"
                    "${PROJECT_SOURCE_DIR}/xlua/xl2c"
                    "${PROJECT_SOURCE_DIR}/xlua/xlpp"
                    "${PROJECT_SOURCE_DIR}/xlua/xlparse"
                    "${PROJECT_SOURCE_DIR}/xlua/xlgen"
                    "${PROJECT_SOURCE_DIR}/xlua/lib/XLua/Parser.pm"
                    "${PROJECT_SOURCE_DIR}/xlua/lib/XLua/Generator.pm")
        set_property(SOURCE "${output_c}" PROPERTY GENERATED TRUE)
        list(APPEND outputs "${output_c}")
    endforeach()
    set("${outputs_name}" "${outputs}" PARENT_SCOPE)
endfunction()


process_xls(rouse_lua_xls ".c" rouse_lua_gen_sources)

add_library(rouse_lua STATIC "${rouse_lua_sources}" "${rouse_lua_gen_sources}"
                             "${rouse_lua_headers}")
set_language_standards(rouse_lua)
target_compile_options(rouse_lua PRIVATE ${rouse_cflags})
target_include_directories(rouse_lua INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(rouse_lua PUBLIC Rouse::rouse lua)
add_asan_if_available(rouse_lua)
add_clang_tidy_if_available(rouse_lua)


if(BUILD_WITH_BOX2D)
    add_rouse_library_raw(box2d "${BOX2D_CFLAGS}" "${BOX2D_LDFLAGS}" "${BOX2D_LIBS}")

    process_xls(rouse_lua_box2d_xls ".cpp" rouse_lua_box2d_gen_sources)

    add_library(rouse_lua_box2d STATIC "${rouse_lua_box2d_sources}"
                                       "${rouse_lua_box2d_gen_sources}"
                                       "${rouse_lua_box2d_headers}")
    set_language_standards(rouse_lua_box2d)
    target_compile_options(rouse_lua_box2d PRIVATE ${rouse_cxxflags})
    target_link_libraries(rouse_lua_box2d PUBLIC Rouse::rouse lua)
    target_link_libraries(rouse_lua_box2d INTERFACE box2d)
    add_asan_if_available(rouse_lua_box2d)
    add_clang_tidy_if_available(rouse_lua_box2d)

    target_compile_definitions(rouse_lua PUBLIC ROUSE_LUA_BOX2D_ENABLED)
    target_link_libraries(rouse_lua PUBLIC rouse_lua_box2d)
endif()
