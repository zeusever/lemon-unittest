#include <lemon/trace/abi.h>
#include <lemonxx/trace/object.hpp>
#include <lemonxx/function/bind.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <unittest/lemon/trace/assembly.h>

namespace lemon{namespace trace{namespace test{

	class ABIUnittest{};

	LEMON_UNITTEST_CASE(ABIUnittest,LemonTraceDescriptionTest)
	{
		//LEMON_CHECK(sizeof(LemonTraceDescription) == 36);
	}

	class TraceUnittest{};

	void Trace(size_t &counter , size_t loop, condition_variable & cv,lemon::uuid_t id,flag_t flag,lemon_tid_t tid,const message & msg)
	{
		description_t description;

		msg.description(description);

		LEMON_CHECK(description.ProcessId == LemonGetProcessID());

		LEMON_CHECK(tid != description.ThreadId);

		LEMON_CHECK(description.Flag == flag);

		LEMON_CHECK(id == *description.Uuid);

		byte_t buffer[256];

		msg.seek(0,lemon::io::seek::begin);

		size_t length = msg.read(buf(buffer));

		LEMON_CHECK(std::string(buffer,buffer + length) == "hello world !!!!");

		++ counter;

		if(counter == loop) cv.notify();
	}

	LEMON_UNITTEST_CASE(TraceUnittest,LocalTest)
	{
		lemon::uuid_t id;

		condition_variable cv;

		mutex_t	mutex;

		flag_t flag = LEMON_MAKE_TRACE_FLAG(LEMON_TL_DEBUG,UNITTEST_LEMON_TRACE_CATALOG_ONE);

		flag_t allflag = LEMON_MAKE_TRACE_FLAG(LEMON_TL_ANY,UNITTEST_LEMON_TRACE_CATALOG_ONE | UNITTEST_LEMON_TRACE_CATALOG_TWO);

		service s;

		size_t counter = 0;

		size_t loop = 100000;

		provider p(s,&id);

		controller c(s);

		conumser cs(c,lemon::bind(&Trace,lemon::ref(counter),loop,lemon::ref(cv),id,flag,current_thread_id(),lemon::_0));

		LEMON_CHECK(!cs.empty());

		conumser cs1;

		LEMON_CHECK(cs1.empty());

		cs1.start(c,lemon::bind(&Trace,lemon::ref(counter),loop,lemon::ref(cv),id,flag,current_thread_id(),lemon::_0));

		LEMON_CHECK(!cs1.empty());

		cs1.close();

		LEMON_CHECK(cs1.empty());

		c.open_trace(&id,allflag);

		mutex_t::scope_lock lock(mutex);

		for(size_t i = 0 ; i < loop; ++ i)
		{
			message_commiter cm(p,flag);

			LEMON_CHECK(!cm.empty());

			cm.write(cbuf("hello world !!!!"));

			cm.commit();

			LEMON_CHECK(cm.empty());
		}

		cv.wait(lock);
		
	}

}}}


