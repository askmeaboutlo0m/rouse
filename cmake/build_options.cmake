option(BUILD_SHARED_LIBS "build shared libraries"                    OFF)
option(BUILD_TESTS       "build tests from the 'test' directory"     ON)
option(BUILD_EXAMPLES    "build examples from 'examples' directory"  ON)
option(BUILD_WITH_MAGIC  "enable magic numbers memory sanity checks" ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG CACHE STRING "debug or release build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")
