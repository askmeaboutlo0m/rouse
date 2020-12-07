if(USE_PKGCONFIG)
    add_rouse_library_pc(cglm        "static"  cglm)
    add_rouse_library_pc(sdl2        "shared"  sdl2 SDL2)
    add_rouse_library_pc(sdl2_image  "shared"  SDL2_image sdl2_image SDL2image sdl2image)
    add_rouse_library_pc(openal      "dynamic" openal)
else()
    add_rouse_library_raw(cglm "${CGLM_CFLAGS}" "${CGLM_LDFLAGS}" "${CGLM_LIBS}")
    add_rouse_library_raw(sdl2 "${SDL2_CFLAGS}" "${SDL2_LDFLAGS}" "${SDL2_LIBS}")
    add_rouse_library_raw(sdl2_image
            "${SDL2_IMAGE_CFLAGS}" "${SDL2_IMAGE_LDFLAGS}" "${SDL2_IMAGE_LIBS}")
    add_rouse_library_raw(openal
            "${OPENAL_CFLAGS}" "${OPENAL_LDFLAGS}" "${OPENAL_LIBS}")
endif()
