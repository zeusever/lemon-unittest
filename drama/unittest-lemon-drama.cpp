#include <iostream>
#include <lemonxx/drama/drama.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace concurrency{namespace test{

	struct LemonDramaUnittest{};

	void StopProc(drama & da)
	{
		da.stop();
	}

	LEMON_UNITTEST_CASE(LemonDramaUnittest,StopTest)
	{
		drama da(10);

		actor mainActor(da,&StopProc);

		da.join();
	}

	void Print(drama & /*da*/)
	{
		std::cout << "actor print ..." << std::endl;
	}

	void JoinProc(drama & da)
	{
		actor actors[10];
		
		for(size_t i = 0 ; i < sizeof(actors)/sizeof(actor); ++ i)
		{
			actors[i] = actor(da,&Print);
		}

		for(size_t i = 0 ; i < sizeof(actors)/sizeof(actor); ++ i)
		{
			actors[i].join();
		}

		da.stop();
	}

	LEMON_UNITTEST_CASE(LemonDramaUnittest,ActorJoinTest)
	{
		{
			
			drama da(10);

			actor mainActor(da,&JoinProc);

			da.join();
		}

		{

			drama da(1);

			actor mainActor(da,&JoinProc);

			da.join();
		}

		{
			drama da(100);

			actor mainActor(da,&JoinProc);

			da.join();
		}
	}
}}}

