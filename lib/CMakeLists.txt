set(rouse_geom_h          "${CMAKE_CURRENT_SOURCE_DIR}/rouse/geom.h")
set(rouse_generate_geom_h "${PROJECT_SOURCE_DIR}/generate-geom-h")

add_custom_command(OUTPUT "${rouse_geom_h}"
    COMMAND "${rouse_generate_geom_h}" ARGS "${rouse_geom_h}"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    DEPENDS "${rouse_generate_geom_h}")

set_property(SOURCE "${rouse_geom_h}" PROPERTY GENERATED TRUE)

add_library(rouse "${rouse_sources}" "${rouse_headers}")

# sources
target_sources(rouse PRIVATE
    rouse/anim/call.c
    rouse/anim/delay.c
    rouse/anim/seq.c
    rouse/anim/util.c
    rouse/camera.c
    rouse/common.c
    rouse/interact/input.c
    rouse/main.c
    rouse/model.c
    rouse/model_parser.c
    rouse/render/binder.c
    rouse/render/frame_buffer.c
    rouse/render/frame_renderer.c
    rouse/render/gl.c
    rouse/render/viewport.c
    rouse/resource.c
    rouse/stringify.c)

# headers
target_sources(rouse PRIVATE
    rouse.h
    rouse/3rdparty/uthash.h
    rouse/3rdparty/uthash_inc.h
    rouse/anim/call.h
    rouse/anim/delay.h
    rouse/anim/seq.h
    rouse/anim/util.h
    rouse/camera.h
    rouse/common.h
    rouse/interact/input.h
    rouse/main.h
    rouse/model.h
    rouse/render/binder.h
    rouse/render/frame_buffer.h
    rouse/render/frame_renderer.h
    rouse/render/gl.h
    rouse/render/viewport.h
    rouse/resource.h
    rouse/sanity.h
    rouse/stringify.h
    "${rouse_geom_h}")

target_include_directories(rouse INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(rouse PRIVATE c_std_11)
target_compile_options(rouse PRIVATE ${rouse_warnings})
target_link_libraries(rouse PUBLIC
    Rouse::cglm
    Rouse::glew
    Rouse::sdl2
    Rouse::sdl2_image)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building with -DROUSE_DEBUG")
    target_compile_definitions(rouse PUBLIC "ROUSE_DEBUG")
endif()

if(BUILD_WITH_MAGIC)
    message(STATUS "Building with -DROUSE_MAGIC")
    target_compile_definitions(rouse PUBLIC "ROUSE_MAGIC")
endif()
