add_subdirectory(3rdparty/lua)


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


function(add_rouse_lua_library name cxx)
    string(TOUPPER "${name}" capitalized_name)
    if(cxx)
        set(ext ".cpp")
        set(flags "${rouse_cxxflags}")
    else()
        set(ext ".c")
        set(flags "${rouse_cflags}")
    endif()

    process_xls("rouse_lua_${name}_xls" "${ext}" "rouse_lua_${name}_gen_sources")

    add_library("rouse_lua_${name}" STATIC "${rouse_lua_${name}_sources}"
                                           "${rouse_lua_${name}_gen_sources}"
                                           "${rouse_lua_${name}_headers}")
    set_language_standards("rouse_lua_${name}")
    target_compile_options("rouse_lua_${name}" PRIVATE ${flags})
    target_link_libraries("rouse_lua_${name}" PUBLIC Rouse::rouse lua "${name}")
    add_asan_if_available("rouse_lua_${name}")
    add_clang_tidy_if_available("rouse_lua_${name}")

    target_compile_definitions(rouse_lua PUBLIC
        "ROUSE_LUA_${capitalized_name}_ENABLED")
    target_link_libraries(rouse_lua PUBLIC "rouse_lua_${name}")
endfunction()

if(BUILD_WITH_BOX2D)
    add_rouse_library_raw(box2d "${BOX2D_CFLAGS}" "${BOX2D_LDFLAGS}" "${BOX2D_LIBS}")
    add_rouse_lua_library(box2d ON)
endif()

if(BUILD_WITH_IMGUI)
    add_subdirectory(3rdparty/imgui)
    add_rouse_lua_library(imgui ON)
endif()

if(BUILD_WITH_POCKETSPHINX)
    add_rouse_library_pc(pocketsphinx shared pocketsphinx)
    add_rouse_lua_library(pocketsphinx OFF)
endif()
