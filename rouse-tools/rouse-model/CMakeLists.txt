cmake_minimum_required(VERSION 3.14)

project(librouse_model
    VERSION 0.2.0
    DESCRIPTION "Rouse model parser"
    LANGUAGES C)


include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/common.cmake")
# include("${CMAKE_CURRENT_LIST_DIR}/../../rouse-lua/cmake/sources.cmake")
# include("${CMAKE_CURRENT_LIST_DIR}/../../rouse-lua/cmake/lua.cmake")

add_rouse_library_pc(assimp dynamic assimp)

add_subdirectory(app)

report_asan_targets()
report_clang_tidy_targets()
