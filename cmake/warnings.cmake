if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(rouse_warnings -pedantic -pedantic-errors -Wall -Wextra -Wshadow
                       -Wstrict-prototypes -Wmissing-include-dirs -Wconversion
                       -Werror -Wno-error=unused-parameter)
    # MinGW links against an ancient MSVC runtime that doesn't support %z
    # modifiers in printf formatting. So we have to use Windows-specific
    # formats in that case, which we don't want to hear warnings about.
    if(WIN32)
        list(APPEND rouse_warnings -Wno-pedantic-ms-format)
    endif()
endif()
