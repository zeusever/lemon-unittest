#include <iostream>
#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/sys/coroutine.hpp>
#include <lemonxx/utility/placeholder.hpp>
#include <lemonxx/function/bind.hpp>

namespace lemon{namespace test{
	struct CoroutineUnittest{};

	class Counter 
	{
	public:
		Counter():_counter(0){}

		void Run(coroutine * self)
		{
			while(_counter < 100000)
			{
				++ _counter;

				self->yield();
			}
		}

		operator size_t ()
		{
			return _counter;
		}

	private:
		size_t _counter;
	};

	LEMON_UNITTEST_CASE(CoroutineUnittest,CoroutineLoopTest)
	{
		Counter counter;

		coroutine parent;

		coroutine child(parent,bind(&Counter::Run,&counter,_0),1024 * 1024);

		while((size_t)counter < 100000)
		{
			child.resume();
		}

	}
}}