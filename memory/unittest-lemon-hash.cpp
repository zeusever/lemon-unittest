#include <hash_map>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace memeory{namespace test{

	class HashMapUnittest{};

	LEMON_UNITTEST_CASE(HashMapUnittest,STLHashMapTest)
	{
		std::hash_map<std::string,int> map;

		map["test"] = 1;
	}

}}}