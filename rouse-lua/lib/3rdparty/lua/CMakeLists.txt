add_library(lua STATIC "${lua_sources}" "${lua_headers}")
set_language_standards(lua)
add_asan_if_available(lua)
target_include_directories(lua INTERFACE "${CMAKE_CURRENT_LIST_DIR}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(lua PUBLIC LUA_USE_APICHECK)
endif()
