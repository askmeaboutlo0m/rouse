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

function(add_rouse_library_target name cflags_var ldflags_var libs_var)
    # We need these targets twice to make `export` work properly. They need to
    # be linked with the Rouse:: prefix and exported without it, otherwise you
    # either get double prefixes or links against targets that don't exist.
    foreach(target IN ITEMS "${name}" "Rouse::${name}")
        add_library("${target}" INTERFACE IMPORTED GLOBAL)
        if(cflags_var)
            target_compile_options("${target}" INTERFACE ${${cflags_var}})
        endif()
        if(ldflags_var)
            target_link_options("${target}" INTERFACE ${${ldflags_var}})
        endif()
        if(libs_var)
            target_link_libraries("${target}" INTERFACE ${${libs_var}})
        endif()
    endforeach()
    report_library_options("${name}")
endfunction()

if(USE_PKGCONFIG)
    find_package(PkgConfig REQUIRED)

    function(add_rouse_library_pc pkg type)
        set(prefix  "")
        set(dynamic OFF)

        if("${type}" STREQUAL "static")
            set(prefix "_STATIC")
        elseif("${type}" STREQUAL "dynamic")
            set(dynamic ON)
        elseif("${type}" STREQUAL "shared")
            # Nothing to do
        else()
            message(FATAL_ERROR "Unknown library type '${type}'")
        endif()

        set(target      "${pkg}")
        set(cflags_var  "${pkg}${prefix}_CFLAGS")

        if(NOT dynamic)
            set(ldflags_var "${pkg}${prefix}_LDFLAGS")
            set(libs_var    "${pkg}${prefix}_LIBS")
        endif()

        pkg_search_module(${pkg} REQUIRED ${ARGN})
        add_rouse_library_target("${target}" "${cflags_var}"
                                 "${ldflags_var}" "${libs_var}")
    endfunction()

    add_rouse_library_pc(cglm        "static"  cglm)
    add_rouse_library_pc(sdl2        "shared"  sdl2 SDL2)
    add_rouse_library_pc(sdl2_image  "shared"  SDL2_image sdl2_image SDL2image sdl2image)
    add_rouse_library_pc(openal      "dynamic" openal)
else()
    function(split_args_to_list var args)
        string(STRIP "${args}" stripped)
        string(REGEX REPLACE "[ \r\n\t]+" ";" replaced "${stripped}")
        set("${var}" "${replaced}" PARENT_SCOPE)
    endfunction()

    function(add_rouse_library_raw pkg cflags ldflags libs)
        set(target "${pkg}")
        split_args_to_list(cflags_list  "${cflags}")
        split_args_to_list(ldflags_list "${ldflags}")
        split_args_to_list(libs_list    "${libs}")
        add_rouse_library_target("${target}" cflags_list ldflags_list libs_list)
    endfunction()

    add_rouse_library_raw(cglm "${CGLM_CFLAGS}" "${CGLM_LDFLAGS}" "${CGLM_LIBS}")
    add_rouse_library_raw(sdl2 "${SDL2_CFLAGS}" "${SDL2_LDFLAGS}" "${SDL2_LIBS}")
    add_rouse_library_raw(sdl2_image
            "${SDL2_IMAGE_CFLAGS}" "${SDL2_IMAGE_LDFLAGS}" "${SDL2_IMAGE_LIBS}")
    add_rouse_library_raw(openal
            "${OPENAL_CFLAGS}" "${OPENAL_LDFLAGS}" "${OPENAL_LIBS}")
endif()
