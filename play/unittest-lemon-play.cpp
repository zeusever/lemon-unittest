#include <iostream>
#include <lemonxx/play/play.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/function/bind.hpp>
#include <lemonxx/sys/sys.hpp>

namespace lemon{namespace concurrency{namespace test{

	struct LemonPlayUnittest{};

	void Hello(atomic_t & i,play &)
	{
		++ i;

		//std::cout << "hello the play world .. " << i << std::endl;
	}

	void Loop(atomic_t & counter,play & p)
	{
		actor actors[200000];

		for(size_t i = 0; i < sizeof(actors)/sizeof(actor); ++ i)
		{
			actors[i] = actor(p,lemon::bind(&Hello,lemon::ref(counter),_0));
		}

		for(size_t i = 0; i < sizeof(actors)/sizeof(actor); ++ i)
		{
			actors[i].join();
		}
	
		p.stop();
	}


	LEMON_UNITTEST_CASE(LemonPlayUnittest,JoinTest)
	{
		{
			play engine(2);

			atomic_t counter;

			lemon::timer_t timer;

			actor loopActor(engine,lemon::bind(&Loop,lemon::ref(counter),_0));

			engine.join();

			std::cout << timer.duration() << std::endl;
		}

		{
			play engine(1);

			atomic_t counter;

			lemon::timer_t timer;

			actor loopActor(engine,lemon::bind(&Loop,lemon::ref(counter),_0));

			engine.join();

			std::cout << timer.duration() << std::endl;
		}
	}

}}}