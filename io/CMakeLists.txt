if(WIN32)
	lemon_c_cxx_files(SRC)
	lemon_unittest_project(unittest-lemon-io ${SRC})
endif(WIN32)