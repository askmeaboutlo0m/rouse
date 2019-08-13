if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(rouse_warnings -pedantic -pedantic-errors -Wall -Wextra -Wshadow
                       -Wstrict-prototypes -Wmissing-include-dirs -Wconversion
                       -Werror -Wno-error=unused-parameter)
endif()
