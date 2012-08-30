#include <lemonxx/io/io.hpp>
#include <lemonxx/memory/smallobj.hpp>
#include <lemonxx/unittest/unittest.hpp>

#include <lemon/io/object.hpp>

namespace lemon{namespace io{namespace test{

	class ObjectUnittest{};

	class TestIOService 
	{
	public:
		void * AllocObj(size_t size)
		{
			return _allocator.alloc(size);
		}

		void FreeObj(void * data, size_t size)
		{
			_allocator.free(data,size);
		}

	private:

		memory::smallobject::allocator<64> _allocator;
	};

	class TestObject : public Object<TestObject,TestIOService>
	{
	public:
		TestObject(TestIOService * service) : Object<TestObject,TestIOService>(service) {}

		//size_t Data[14];
	};

	LEMON_UNITTEST_CASE(ObjectUnittest,AllocFreeTest)
	{
		TestIOService service;

		for (size_t i = 0; i < 1000000 ; ++ i)
		{
			TestObject * obj = new(&service) TestObject(&service);

			delete obj;
		}
	}
}}}
