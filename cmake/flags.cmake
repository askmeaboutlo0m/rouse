if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(rouse_common_warnings -pedantic -pedantic-errors -Wall -Wextra -Wshadow
                              -Wmissing-include-dirs -Wconversion -Werror
                              -Wno-error=unused-parameter)

    # MinGW links against an ancient MSVC runtime that doesn't support %z
    # modifiers in printf formatting. So we have to use Windows-specific
    # formats in that case, which we don't want to hear warnings about.
    if(CMAKE_C_COMPILER MATCHES "mingw")
        list(APPEND rouse_common_warnings -Wno-pedantic-ms-format)
    endif()

    set(rouse_cflags   ${rouse_common_warnings} -Wstrict-prototypes)
    set(rouse_cxxflags ${rouse_common_warnings} -fno-exceptions -fno-rtti)
endif()
