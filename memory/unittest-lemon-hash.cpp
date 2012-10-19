#include <hash_map>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace memeory{namespace test{

	class HashMapUnittest{};

	LEMON_UNITTEST_CASE(HashMapUnittest,STLHashMapTest)
	{
		std::hash_map<int,int> map;

		map[64] = 1;
	}

}}}