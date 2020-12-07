add_library(lua STATIC "${lua_sources}" "${lua_headers}")
set_target_properties(lua PROPERTIES OUTPUT_NAME rouse_lua_5_3_5)
set_language_standards(lua)
add_asan_if_available(lua)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(lua PUBLIC LUA_USE_APICHECK)
endif()


set(xl2c "${PROJECT_SOURCE_DIR}/xlua/xl2c")
set(rouse_lua_gen_sources)

foreach(input_xl IN LISTS rouse_lua_xls)
    string(REGEX REPLACE "\\.xl$" ".c" output_c "${input_xl}")
    add_custom_command(OUTPUT "${output_c}"
        COMMAND "${xl2c}" ARGS "${input_xl}" "${output_c}"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        DEPENDS "${input_xl}"
                "${PROJECT_SOURCE_DIR}/xlua/xl2c"
                "${PROJECT_SOURCE_DIR}/xlua/xlpp"
                "${PROJECT_SOURCE_DIR}/xlua/xlparse"
                "${PROJECT_SOURCE_DIR}/xlua/xlgen"
                "${PROJECT_SOURCE_DIR}/xlua/lib/XLua/Parser.pm"
                "${PROJECT_SOURCE_DIR}/xlua/lib/XLua/Generator.pm")
    set_property(SOURCE "${output_c}" PROPERTY GENERATED TRUE)
    list(APPEND rouse_lua_gen_sources "${output_c}")
endforeach()


add_library(rouse_lua STATIC "${rouse_lua_sources}" "${rouse_lua_gen_sources}"
                             "${rouse_lua_headers}")
set_language_standards(rouse_lua)
target_compile_options(rouse_lua PRIVATE ${rouse_cflags})
target_include_directories(rouse_lua INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(rouse_lua PUBLIC Rouse::rouse lua)
add_asan_if_available(rouse_lua)
add_clang_tidy_if_available(rouse_lua)
