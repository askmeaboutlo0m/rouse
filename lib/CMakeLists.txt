# Parson spews compiler warnings and clang-tidy violations.
# So it's its own separate target without both of those.
# I guess that's okay since it's only really used for debugging.
# Maybe it should be excluded in release mode even?
add_library(parson STATIC "${parson_sources}" "${parson_headers}")
set_target_properties(parson PROPERTIES OUTPUT_NAME rouse_parson)
set_c11_standard(parson)
add_asan_if_available(parson)

# Same for NanoVG, needs to be a separate target to avoid warnings.
add_library(nanovg STATIC "${nanovg_sources}" "${nanovg_headers}")
set_target_properties(nanovg PROPERTIES OUTPUT_NAME rouse_nanovg)
set_c11_standard(nanovg)
add_asan_if_available(nanovg)


set(rouse_geom_h          "${CMAKE_CURRENT_SOURCE_DIR}/rouse/geom.h")
set(rouse_generate_geom_h "${PROJECT_SOURCE_DIR}/generate-geom-h")

add_custom_command(OUTPUT "${rouse_geom_h}"
    COMMAND "${rouse_generate_geom_h}" ARGS "${rouse_geom_h}"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    DEPENDS "${rouse_generate_geom_h}")

set_property(SOURCE "${rouse_geom_h}" PROPERTY GENERATED TRUE)


add_library(rouse "${rouse_sources}" "${rouse_headers}")
set_c11_standard(rouse)
target_include_directories(rouse INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(rouse PUBLIC "${CMAKE_BINARY_DIR}/config")
target_compile_options(rouse PRIVATE ${rouse_warnings})
add_clang_tidy_if_available(rouse)
add_asan_if_available(rouse)
target_link_libraries(rouse PUBLIC nanovg parson cglm sdl2 sdl2_image)
