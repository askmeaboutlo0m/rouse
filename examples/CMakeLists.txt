set(cube_sources cube/cube.c common.c common.h)
set(skel_sources skel/skel.c common.c common.h)
set(ease_sources ease/ease.c)

foreach(exe IN ITEMS cube skel ease)
    add_rouse_linked_executable(${exe} ${${exe}_sources})

    if(ROUSE_EMSCRIPTEN)
        set_property(TARGET "${exe}" PROPERTY SUFFIX ".html")
        file(STRINGS "${exe}/preload-files" "${exe}_preloads")
        foreach(preload IN LISTS "${exe}_preloads")
            target_link_options("${exe}" PUBLIC
                "--preload-file=${PROJECT_SOURCE_DIR}/${preload}@${preload}")
        endforeach()
    endif()
endforeach()
