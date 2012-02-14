#include <iostream>
#include <lemonxx/sys/sys.hpp>
#include <lemonxx/memory/smallobj.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace memory{namespace test{

	class Object
	{
	public:
		virtual ~Object(){}
	};
	
	class SmallObjUnittest : public Object
	{
		char buffer[1];
	};

	class SmallObject : public smallobj
	{
		char buffer[1];
	};

	LEMON_UNITTEST_CASE(SmallObjUnittest,PerformanceTest)
	{
		lemon::timer_t timer;

		static const size_t counter = 1024;

		SmallObjUnittest * objects[counter] = {0};

		SmallObject * smallObjects[counter] = {0};

		timer.reset();

		for(size_t j = 0 ; j < counter ; ++ j)
		{
			for(size_t i = 0 ; i < counter ; ++ i)
			{
				smallObjects[i] = new SmallObject();

				delete smallObjects[i];
			}
		}

		time_duration duration = timer.duration();

		std::cout << "call smallobj new operator:" << duration << std::endl;

		timer.reset();

		for(size_t j = 0 ; j < counter ; ++ j)
		{
			for(size_t i = 0 ; i < counter ; ++ i)
			{
				objects[i] = new SmallObjUnittest();

				delete objects[i];
			}
		}
		

		duration = timer.duration();

		std::cout << "call c++ buildin new operator:" << duration << std::endl;
	}

}}}