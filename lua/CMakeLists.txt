lemon_c_cxx_files(SRC)
lemon_unittest_project(unittest-lemon-lua ${SRC})

target_link_libraries(unittest-lemon-lua lemon-sys)