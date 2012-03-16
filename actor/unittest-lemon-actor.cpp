#include <iostream>
#include <lemonxx/actor/actor.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace concurrency{namespace test{

	struct EngineUnittest{};

	void Process(actor_engine&)
	{
		std::cout << "test ..." << std::endl;
	}

	void ActorMain(actor_engine& engine)
	{
		LemonActor actors[10] = {0};

		for(size_t i = 0; i < sizeof(actors)/sizeof(LemonActor); ++ i)
		{
			engine.create_actor(&Process);
		}

		for(size_t i = 0; i < sizeof(actors)/sizeof(LemonActor); ++ i)
		{
			engine.join_actor(actors[i]);
		}

		engine.stop();
	}

	

	LEMON_UNITTEST_CASE(EngineUnittest,CreateActorTests)
	{
		actor_engine eg(10);

		eg.create_actor(&ActorMain);

		eg.join();
	}

}}}
