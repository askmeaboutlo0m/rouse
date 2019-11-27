# Parson spews compiler warnings and clang-tidy violations.
# So it's its own separate target without both of those.
# I guess that's okay since it's only really used for debugging.
# Maybe it should be excluded in release mode even?
add_library(parson STATIC rouse/3rdparty/parson.c)
set_c11_standard(parson)
add_asan_if_available(parson)

# Same for NanoVG, needs to be a separate target to avoid warnings.
add_library(nanovg STATIC)
set_c11_standard(nanovg)
add_asan_if_available(nanovg)
target_sources(nanovg PRIVATE
    rouse/3rdparty/nanovg_gles2.c
    rouse/3rdparty/nanovg/fontstash.h
    rouse/3rdparty/nanovg/nanovg.c
    rouse/3rdparty/nanovg/nanovg_gl.h
    rouse/3rdparty/nanovg/nanovg_gl_utils.h
    rouse/3rdparty/nanovg/nanovg.h
    rouse/3rdparty/nanovg/stb_image.h
    rouse/3rdparty/nanovg/stb_truetype.h)


set(rouse_geom_h          "${CMAKE_CURRENT_SOURCE_DIR}/rouse/geom.h")
set(rouse_generate_geom_h "${PROJECT_SOURCE_DIR}/generate-geom-h")

add_custom_command(OUTPUT "${rouse_geom_h}"
    COMMAND "${rouse_generate_geom_h}" ARGS "${rouse_geom_h}"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    DEPENDS "${rouse_generate_geom_h}")

set_property(SOURCE "${rouse_geom_h}" PROPERTY GENERATED TRUE)


add_library(rouse)

# sources
target_sources(rouse PRIVATE
    rouse/2d/bitmap.c
    rouse/2d/canvas.c
    rouse/2d/nvg.c
    rouse/2d/sprite.c
    rouse/2d/sprite_anim.c
    rouse/2d/text.c
    rouse/2d/vector.c
    rouse/anim/call.c
    rouse/anim/delay.c
    rouse/anim/ease.c
    rouse/anim/seq.c
    rouse/anim/tween.c
    rouse/anim/util.c
    rouse/camera.c
    rouse/common.c
    rouse/interact/input.c
    rouse/json.c
    rouse/main.c
    rouse/model.c
    rouse/parse.c
    rouse/render/binder.c
    rouse/render/frame_buffer.c
    rouse/render/frame_renderer.c
    rouse/render/gl.c
    rouse/render/viewport.c
    rouse/resource.c
    rouse/string.c
    rouse/stringify.c)

# headers
target_sources(rouse PRIVATE
    rouse.h
    rouse/2d/bitmap.h
    rouse/2d/canvas.h
    rouse/2d/nvg.h
    rouse/2d/refcount.h
    rouse/2d/sprite.h
    rouse/2d/sprite_anim.h
    rouse/2d/text.h
    rouse/2d/vector.h
    rouse/3rdparty/nanovg_inc.h
    rouse/3rdparty/parson.h
    rouse/3rdparty/uthash.h
    rouse/3rdparty/uthash_inc.h
    rouse/anim/call.h
    rouse/anim/delay.h
    rouse/anim/ease.h
    rouse/anim/seq.h
    rouse/anim/tween.h
    rouse/anim/util.h
    rouse/camera.h
    rouse/common.h
    rouse/interact/input.h
    rouse/json.h
    rouse/main.h
    rouse/model.h
    rouse/parse.h
    rouse/render/binder.h
    rouse/render/frame_buffer.h
    rouse/render/frame_renderer.h
    rouse/render/gl.h
    rouse/render/viewport.h
    rouse/resource.h
    rouse/sanity.h
    rouse/string.h
    rouse/stringify.h
    "${rouse_geom_h}")

add_clang_tidy_if_available(rouse)
add_asan_if_available(rouse)

target_include_directories(rouse INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
set_c11_standard(rouse)
target_compile_options(rouse PRIVATE ${rouse_warnings})
target_link_libraries(rouse PUBLIC
    nanovg
    parson
    Rouse::cglm
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
