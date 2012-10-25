#include <lemon/io/hashmap.h>
#include <lemonxx/sys/sys.hpp>
#include <lemonxx/memory/fixobj.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <unittest/lemon/io/configure.h>

#ifdef UNITTEST_LEMON_IO_HAS_HASH_MAP
#include <hash_map>
#endif //UNITTEST_LEMON_IO_HAS_HASH_MAP


namespace lemon{namespace io{namespace test{

	struct HashMapUnittest
	{
		static const size_t Counter = 1048576;
	};

	class HashMapUnittestDumpWriter : public LemonIoWriter , private lemon::nocopyable
	{
	public:
		
		HashMapUnittestDumpWriter(std::ostream & stream):_stream(stream) 
		{
			LemonIoWriter::UserData = this;

			LemonIoWriter::Write = &HashMapUnittestDumpWriter::StreamWrite;
		}

	private:

		static size_t StreamWrite(void * userdata,const lemon_byte_t * source,size_t length,LemonErrorInfo * /*errorCode*/)
		{
			reinterpret_cast<HashMapUnittestDumpWriter*>(userdata)->_stream.write((const char*)source,length);

			return length;
		}

	private:

		std::ostream			&_stream;
	};

	LEMON_UNITTEST_CASE(HashMapUnittest,HashMapInsertDumpTest)
	{
		LEMON_DECLARE_ERRORINFO(errorCode);

		LemonIOHashMap map = LemonCreateIOHashMap(&errorCode);

		lemon::memory::fixed::allocator<LEMON_HANDLE_IMPLEMENT_SIZEOF(LemonIO)> allocator;

		lemon::random_t random;

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			LemonIO io = (LemonIO)allocator.alloc();

			io->Handle = (__lemon_io_file)random.next();

			LemonIOHashMapInsert(map,io,&errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));
		}

		std::cout << "max collision chain length :" << LemonIOHashMapMaxCollisionChainLength(map) << std::endl;

		std::ofstream file("LemonIOHashMapInsertDump.db",std::ios::trunc | std::ios::binary);

		HashMapUnittestDumpWriter writer(file);

		LemonIOHashMapDump(map,&writer,&errorCode);

		LemonReleaseIOHashMap(map);
	}

	LEMON_UNITTEST_CASE(HashMapUnittest,HashMapInsertTest)
	{
		LEMON_DECLARE_ERRORINFO(errorCode);

		LemonIOHashMap map = LemonCreateIOHashMap(&errorCode);

		lemon::memory::fixed::allocator<LEMON_HANDLE_IMPLEMENT_SIZEOF(LemonIO)> allocator;

		lemon::random_t random;

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			LemonIO io = (LemonIO)allocator.alloc();

			io->Handle = (__lemon_io_file)random.next();

			LemonIOHashMapInsert(map,io,&errorCode);

			//LEMON_CHECK(LEMON_SUCCESS(errorCode));
		}

		/*std::cout << "max collision chain length :" << LemonIOHashMapMaxCollisionChainLength(map) << std::endl;

		std::ofstream file("LemonIOHashMap.db",std::ios::trunc | std::ios::binary);

		HashMapUnittestDumpWriter writer(file);

		LemonIOHashMapDump(map,&writer,&errorCode);*/

		LemonReleaseIOHashMap(map);
	}

	LEMON_UNITTEST_CASE(HashMapUnittest,HashMapSearchTest)
	{
		LEMON_DECLARE_ERRORINFO(errorCode);

		LemonIOHashMap map = LemonCreateIOHashMap(&errorCode);

		lemon::memory::fixed::allocator<LEMON_HANDLE_IMPLEMENT_SIZEOF(LemonIO)> allocator;

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			LemonIO io = (LemonIO)allocator.alloc();

			io->Handle = (__lemon_io_file)i;

			LemonIOHashMapInsert(map,io,&errorCode);

			//LEMON_CHECK(LEMON_SUCCESS(errorCode));
		}

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			LEMON_CHECK(LEMON_HANDLE_NULL_VALUE != LemonIOHashMapSearch(map,(__lemon_io_file)i));
		}
		
		LemonReleaseIOHashMap(map);
	}

	LEMON_UNITTEST_CASE(HashMapUnittest,HashMapRemoveTest)
	{
		LEMON_DECLARE_ERRORINFO(errorCode);

		LemonIOHashMap map = LemonCreateIOHashMap(&errorCode);

		lemon::memory::fixed::allocator<LEMON_HANDLE_IMPLEMENT_SIZEOF(LemonIO)> allocator;

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			LemonIO io = (LemonIO)allocator.alloc();

			io->Handle = (__lemon_io_file)i;

			LemonIOHashMapInsert(map,io,&errorCode);

			//LEMON_CHECK(LEMON_SUCCESS(errorCode));
		}

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			LEMON_CHECK(LEMON_HANDLE_NULL_VALUE != LemonIOHashMapRemove(map,(__lemon_io_file)i));
		}

		LemonReleaseIOHashMap(map);
	}


#ifdef UNITTEST_LEMON_IO_HAS_HASH_MAP
	LEMON_UNITTEST_CASE(HashMapUnittest,STLHashMapInsertTest)
	{
		lemon::memory::fixed::allocator<LEMON_HANDLE_IMPLEMENT_SIZEOF(LemonIO)> allocator;

		std::hash_map<__lemon_io_file,LemonIO> map;

		lemon::random_t random;

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			LemonIO io = (LemonIO)allocator.alloc();

			io->Handle = (__lemon_io_file)random.next();

			map[(__lemon_io_file)i] = io;
		}
	}

	LEMON_UNITTEST_CASE(HashMapUnittest,STLHashMapSearchTest)
	{
		lemon::memory::fixed::allocator<LEMON_HANDLE_IMPLEMENT_SIZEOF(LemonIO)> allocator;

		std::hash_map<__lemon_io_file,LemonIO> map;

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			LemonIO io = (LemonIO)allocator.alloc();

			io->Handle = (__lemon_io_file)i;

			map[(__lemon_io_file)i] = io;
		}

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			LEMON_CHECK(LEMON_HANDLE_NULL_VALUE != map[(__lemon_io_file)i]);
		}

	}

	LEMON_UNITTEST_CASE(HashMapUnittest,STLHashMapRemoveTest)
	{
		lemon::memory::fixed::allocator<LEMON_HANDLE_IMPLEMENT_SIZEOF(LemonIO)> allocator;

		std::hash_map<__lemon_io_file,LemonIO> map;

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			LemonIO io = (LemonIO)allocator.alloc();

			io->Handle = (__lemon_io_file)i;

			map[(__lemon_io_file)i] = io;
		}

		for(size_t i = 0; i < Context()->Counter; ++ i)
		{
			map.erase((__lemon_io_file)i);
		}
	}
#endif //UNITTEST_LEMON_IO_HAS_HASH_MAP

}}}
