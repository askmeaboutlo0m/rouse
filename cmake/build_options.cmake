option(BUILD_SHARED_LIBS "build shared libraries"                             OFF)
option(BUILD_WITH_MAGIC  "enable magic numbers memory sanity checks"          ON)
option(USE_ASAN          "use address sanitizer for all targets if available" ON)
option(USE_CLANG_TIDY    "use clang-tidy for all targets if available"        ON)
option(BUILD_WITH_OPENAL "enable OpenAL support for audio playback"           ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "debug or release build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")
