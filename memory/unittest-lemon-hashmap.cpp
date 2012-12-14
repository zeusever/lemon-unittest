#include <lemonxx/memory/hashmap.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <unittest/lemon/memory/configure.h>

#ifdef HAS_STL_HASH_MAP
#include <hash_map>
#endif //HAS_STL_HASH_MAP

namespace lemon{namespace memory{namespace test{

	class HashMapUnittest{};

	size_t Hash(const void* key){ return (size_t)key; }

	int Compare(const void* l, const void* r) 
	{
		size_t lhs = *(size_t*)l;

		size_t rhs = *(size_t*)r;

		if(lhs > rhs) return 1;

		if(lhs == rhs) return 0;

		return -1;
	}

	LEMON_UNITTEST_CASE(HashMapUnittest,SizeKeyTest)
	{
		error_info errorCode;

		LemonHashMap map = LemonCreateHashMap(1024,0.5f,&Hash,&Compare,&errorCode);

		LEMON_CHECK(map != LEMON_HANDLE_NULL_VALUE);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		size_t buffer[1024 * 1024];

		for(size_t i = 0; i < sizeof(buffer)/sizeof(size_t); ++ i)
		{
			buffer[i] = i;

			LemonHashMapAdd(map,&buffer[i],&buffer[i],errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));
		}

		for(size_t i = 0; i < sizeof(buffer)/sizeof(size_t); ++ i)
		{
			 LEMON_CHECK(&buffer[i] == LemonHashMapSearch(map,&buffer[i]));

			LEMON_CHECK(LEMON_SUCCESS(errorCode));
		}

		for(size_t i = 0; i < sizeof(buffer)/sizeof(size_t); ++ i)
		{
			LEMON_CHECK(&buffer[i] == LemonHashMapRemove(map,&buffer[i]));

			LEMON_CHECK(LEMON_SUCCESS(errorCode));
		}

		LemonReleaseHashMap(map);
	}

#ifdef HAS_STL_HASH_MAP
	LEMON_UNITTEST_CASE(HashMapUnittest,STLSizeKeyTest)
	{
		std::hash_map<size_t,size_t> map;

		size_t buffer[1024 * 1024];

		for(size_t i = 0; i < sizeof(buffer)/sizeof(size_t); ++ i)
		{
			buffer[i] = i;

			map[buffer[i]] = buffer[i];
		}

		for(size_t i = 0; i < sizeof(buffer)/sizeof(size_t); ++ i)
		{
			LEMON_CHECK(buffer[i] == map[buffer[i]]);
		}

		for(size_t i = 0; i < sizeof(buffer)/sizeof(size_t); ++ i)
		{
			map.erase(buffer[i]);
		}
	}
#endif //HAS_STL_HASH_MAP

}}}
