add_executable(rmodel "${CMAKE_CURRENT_LIST_DIR}/rmodel.cpp")
set_language_standards(rmodel)
target_compile_options(rmodel PRIVATE ${rouse_cxxflags})
target_link_libraries(rmodel PUBLIC -lm -lassimp)
add_asan_if_available(rmodel)
add_clang_tidy_if_available(rmodel)
set_target_properties(rmodel PROPERTIES OUTPUT_NAME "rmodel")
