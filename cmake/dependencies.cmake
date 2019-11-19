function(maybe_get_target_property var target name)
    get_target_property(value "${target}" "${name}")
    if(value)
        set("${var}" "${value}" PARENT_SCOPE)
    else()
        unset("${var}" PARENT_SCOPE)
    endif()
endfunction()

function(report_library_options target)
    maybe_get_target_property(cflags  "${target}" INTERFACE_COMPILE_OPTIONS)
    maybe_get_target_property(ldflags "${target}" INTERFACE_LINK_OPTIONS)
    maybe_get_target_property(libs    "${target}" INTERFACE_LINK_LIBRARIES)
    message(STATUS "${target} compile options: ${cflags}")
    message(STATUS "${target} link options: ${ldflags}")
    message(STATUS "${target} link libraries: ${libs}")
endfunction()

if(USE_PKGCONFIG)
    find_package(PkgConfig REQUIRED)

    function(add_rouse_library_pc pkg prefix)
        set(target      "Rouse::${pkg}")
        set(cflags_var  "${pkg}${prefix}_CFLAGS")
        set(ldflags_var "${pkg}${prefix}_LDFLAGS")

        pkg_search_module(${pkg} REQUIRED ${ARGN})
        add_library("${target}" INTERFACE IMPORTED GLOBAL)
        target_compile_options("${target}" INTERFACE ${${cflags_var}})
        target_link_options(   "${target}" INTERFACE ${${ldflags_var}})
        report_library_options("${target}")
    endfunction()

    add_rouse_library_pc(cglm "_STATIC" cglm)
    add_rouse_library_pc(glew        "" glew GLEW)
    add_rouse_library_pc(sdl2        "" sdl2 SDL2)
    add_rouse_library_pc(sdl2_image  "" SDL2_image sdl2_image SDL2image sdl2image)
else()
    function(split_args_to_list var args)
        string(STRIP "${args}" stripped)
        string(REGEX REPLACE "[ \r\n\t]+" ";" replaced "${stripped}")
        set("${var}" "${replaced}" PARENT_SCOPE)
    endfunction()

    function(add_rouse_library_raw pkg cflags ldflags libs)
        set(target "Rouse::${pkg}")
        split_args_to_list(cflags_list  "${cflags}")
        split_args_to_list(ldflags_list "${ldflags}")
        split_args_to_list(libs_list    "${libs}")

        add_library("${target}" INTERFACE IMPORTED GLOBAL)
        target_compile_options("${target}" INTERFACE "${cflags_list}")
        target_link_options(   "${target}" INTERFACE "${ldflags_list}")
        target_link_libraries( "${target}" INTERFACE "${libs_list}")
        report_library_options("${target}")
    endfunction()

    add_rouse_library_raw(cglm "${CGLM_CFLAGS}" "${CGLM_LDFLAGS}" "${CGLM_LIBS}")
    add_rouse_library_raw(glew "${GLEW_CFLAGS}" "${GLEW_LDFLAGS}" "${GLEW_LIBS}")
    add_rouse_library_raw(sdl2 "${SDL2_CFLAGS}" "${SDL2_LDFLAGS}" "${SDL2_LIBS}")
    add_rouse_library_raw(sdl2_image
            "${SDL2_IMAGE_CFLAGS}" "${SDL2_IMAGE_LDFLAGS}" "${SDL2_IMAGE_LIBS}")
endif()
