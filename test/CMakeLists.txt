set(test_names common model resource viewport)

if(ROUSE_EMSCRIPTEN)
    find_program(node NAMES node nodejs)
endif()

foreach(test IN LISTS test_names)
    add_rouse_linked_executable("${test}" "${test}.c")

    if(ROUSE_EMSCRIPTEN)
        set_property(TARGET "${test}" PROPERTY SUFFIX ".js")
        target_link_options("${test}" PUBLIC "-sEXIT_RUNTIME=1"
            "--embed-file=${PROJECT_SOURCE_DIR}/test/data@test/data")
        set("${test}_args" "${node}" $<TARGET_FILE:${test}>)
    else()
        set("${test}_args" "${test}")
    endif()

    add_test(NAME "${test}" COMMAND ${${test}_args}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endforeach()
