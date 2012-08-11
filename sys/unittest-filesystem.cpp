#include <lemonxx/sys/filesystem.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace fs{namespace test{

	struct FileSystemUnittest{};

	LEMON_UNITTEST_CASE(FileSystemUnittest,exist_Test)
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
	}

	LEMON_UNITTEST_CASE(FileSystemUnittest,path_test)
	{
		fs::path win32Path1(LEMON_TEXT("c:\\test.txt"));

		fs::path win32Path2(LEMON_TEXT(".\\test.txt"));

		fs::path unixPath1(LEMON_TEXT("./test.txt"));

		fs::path unixPath2(LEMON_TEXT("/test/../a/./../test.txt"));

		LEMON_CHECK(win32Path1.root() == LEMON_TEXT("c:"));

		LEMON_CHECK(win32Path2.root() == LEMON_TEXT("."));

		LEMON_CHECK(unixPath1.root() == LEMON_TEXT("."));

		LEMON_CHECK(win32Path1.begin() != win32Path1.end());

		LEMON_CHECK(*win32Path1.begin()  ==  LEMON_TEXT("test.txt"));

		LEMON_CHECK(unixPath1.leaf()  ==  LEMON_TEXT("test.txt"));

		LEMON_CHECK(win32Path2.has_leaf());

		LEMON_CHECK(win32Path2.relative_path());

		LEMON_CHECK(unixPath1.relative_path());

		LEMON_CHECK(extension(unixPath1) == LEMON_TEXT("txt"));
		
		unixPath2.compress();

		LEMON_CHECK(unixPath2.string() == LEMON_TEXT("/test.txt"));

		{
			fs::path a(LEMON_TEXT("/a/"));

			fs::path b(LEMON_TEXT("/a/b"));

			LEMON_CHECK(relative(a,b) == fs::path(LEMON_TEXT("./b")));
		}

		{
			fs::path a(LEMON_TEXT("/a/c/d"));

			fs::path b(LEMON_TEXT("/a/b"));

			LEMON_CHECK(relative(a,b) == fs::path(LEMON_TEXT("../../b")));
		}

		{
			fs::path a(LEMON_TEXT("c:/a/c/d"));

			fs::path b(LEMON_TEXT("c:/a/b"));

			LEMON_CHECK(relative(a,b) == fs::path(LEMON_TEXT("../../b")));
		}

		{
			fs::path a(LEMON_TEXT("c:/a/c/d"));

			fs::path b(LEMON_TEXT("d:/a/b"));

			LEMON_CHECK(relative(a,b) == fs::path());
		}
	}

}}}
