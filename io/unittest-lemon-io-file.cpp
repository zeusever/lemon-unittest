#include <sstream>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{

	struct FileUnittest{};

	/*LEMON_UNITTEST_CASE(FileUnittest,FileSystemTest)
	{
		String current = current_directory();

		current_directory(current);

		LEMON_CHECK(current_directory() == current);

		String dirName = LEMON_TEXT("{C3F0C7DD-A2A8-42B5-BF87-277345DE9774}");

		if(exists(dirName))
		{
			if(is_directory(dirName)) remove_directories(dirName);

			else remove_file(dirName);
		}

		create_directory(dirName);

		LEMON_CHECK(exists(dirName));

		const static size_t children = 100;

		for(size_t i = 0; i < children ; ++ i)
		{
			StringStream stream;

			stream << LEMON_TEXT("{C3F0C7DD-A2A8-42B5-BF87-277345DE9774}/") << i;

			String currentFile = stream.str();

			create_directory(currentFile);

			LEMON_CHECK(exists(currentFile));
		}

		directory_iteartor_t iter(dirName);

		directory_iteartor_t end;

		size_t i;

		for(i = 0;iter != end; ++ iter,++i)
		{
			if(LEMON_TEXT(".") == *iter) continue;

			if(LEMON_TEXT("..") == *iter) continue;

			String path = dirName + LEMON_TEXT("/") + *iter;

			remove_directory(path);

			LEMON_CHECK(!exists(path));
		}

		LEMON_CHECK(children == (i - 2));

		remove_directory(dirName);

		LEMON_CHECK(!exists(dirName));
	}*/
}}