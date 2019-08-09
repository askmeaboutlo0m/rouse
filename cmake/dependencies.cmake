find_package(PkgConfig REQUIRED)

function(add_rouse_library pkg prefix)
    set(target      "Rouse::${pkg}")
    set(cflags_var  "${pkg}${prefix}_CFLAGS")
    set(ldflags_var "${pkg}${prefix}_LDFLAGS")

    pkg_search_module(${pkg} REQUIRED ${ARGN})
    add_library("${target}" INTERFACE IMPORTED GLOBAL)
    target_compile_options("${target}" INTERFACE ${${cflags_var}})
    target_link_options(   "${target}" INTERFACE ${${ldflags_var}})
endfunction()

add_rouse_library(cglm "_STATIC" cglm-static)
add_rouse_library(glew        "" glew GLEW)
add_rouse_library(sdl2        "" sdl2 SDL2)
add_rouse_library(sdl2_image  "" SDL2_image sdl2_image SDL2image sdl2image)
