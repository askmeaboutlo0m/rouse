# Parson spews compiler warnings and clang-tidy violations.
# So it's its own separate target without both of those.
# I guess that's okay since it's only really used for debugging.
# Maybe it should be excluded in release mode even?
add_library(parson STATIC "${parson_sources}" "${parson_headers}")
set_target_properties(parson PROPERTIES OUTPUT_NAME rouse_parson)
set_language_standards(parson)
add_asan_if_available(parson)

# Same for NanoVG, needs to be a separate target to avoid warnings.
add_library(nanovg STATIC "${nanovg_sources}" "${nanovg_headers}")
set_target_properties(nanovg PROPERTIES OUTPUT_NAME rouse_nanovg)
set_language_standards(nanovg)
add_asan_if_available(nanovg)

set(rouse_all_sources "${rouse_sources}" "${rouse_headers}")

# And stb_vorbis as well. But we want to conditionally compile that anyway.
if(BUILD_WITH_OPENAL)
    add_library(stb_vorbis STATIC "${stb_vorbis_sources}" "${stb_vorbis_headers}")
    set_target_properties(stb_vorbis PROPERTIES OUTPUT_NAME rouse_stb_vorbis)
    target_compile_definitions(stb_vorbis PUBLIC STB_VORBIS_NO_PUSHDATA_API
                               STB_VORBIS_NO_FAST_SCALED_FLOAT)
    set_language_standards(stb_vorbis)
    add_asan_if_available(stb_vorbis)

    set(rouse_openal_libs stb_vorbis Rouse::openal)
    if(BUILD_WITH_AL_DL)
        list(APPEND rouse_openal_libs ${CMAKE_DL_LIBS})
    endif()

    list(APPEND rouse_all_sources "${rouse_audio_sources}" "${rouse_audio_headers}")
else()
    unset(rouse_openal_libs)
endif()

message(STATUS "rouse_openal_libs: ${rouse_openal_libs}")


set(rouse_geom_h          "${CMAKE_CURRENT_SOURCE_DIR}/rouse/geom.h")
set(rouse_generate_geom_h "${PROJECT_SOURCE_DIR}/generate-geom-h")

add_custom_command(OUTPUT "${rouse_geom_h}"
    COMMAND "${rouse_generate_geom_h}" ARGS "${rouse_geom_h}"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    DEPENDS "${rouse_generate_geom_h}")

set_property(SOURCE "${rouse_geom_h}" PROPERTY GENERATED TRUE)


add_library(rouse "${rouse_all_sources}")
set_language_standards(rouse)
target_include_directories(rouse INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(rouse PUBLIC "${CMAKE_BINARY_DIR}/config")
target_compile_options(rouse PRIVATE ${rouse_cflags})
add_clang_tidy_if_available(rouse)
add_asan_if_available(rouse)
target_link_libraries(rouse PUBLIC nanovg parson ${rouse_openal_libs}
                                   Rouse::cglm Rouse::sdl2 Rouse::sdl2_image)
