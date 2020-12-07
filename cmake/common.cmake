if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling for platform '${CMAKE_SYSTEM_NAME}'")
endif()

if(CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file '${CMAKE_TOOLCHAIN_FILE}'")
endif()

include("${CMAKE_CURRENT_LIST_DIR}/build_options.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/dependencies.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/flags.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/asan.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/clang_tidy.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/language_standards.cmake")
