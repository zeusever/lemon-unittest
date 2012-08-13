#include <lemonxx/diagnosis/trace.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <unittest/lemon/diagnosis/assembly.h>
#include <unittest/lemon/diagnosis/unittest-lemon-trace.cpp.g.hpp>

namespace lemon{namespace dtrace{namespace test{

	struct DTraceUnittest
	{
		service		Service;

		provider	Provider;

		DTraceUnittest()
			:Provider(Service,&UNITTEST_LEMON_DIAGNOSIS_GUID)
		{
			
		}
	};

	void Hello(lemon::mutex_t & mutex,condition_variable & cv,const message & e)
	{
		char buffer[256];

		size_t length = e.read_rawdata(lemon::buf(buffer));

		LEMON_CHECK(std::string(buffer,buffer + length) == "hello the world");

		lemon::mutex_t::scope_lock lock(mutex);

		cv.notify();
	}

	LEMON_UNITTEST_CASE(DTraceUnittest,ProviderTest)
	{
		lemon::condition_variable cv;

		lemon::mutex_t mutex;

		lemon::mutex_t::scope_lock lock(mutex);

		LemonDTraceFlags flags;

		flags.S.Catalog = LEMON_SYS_FILESYSTEM | LEMON_SYS_COROUTINE;

		flags.S.Level = LEMON_TRACELOG_DEBUG;

		controller c(Context()->Service);

		consumer cs(c,lemon::bind(&Hello,lemon::ref(mutex),lemon::ref(cv),lemon::_0));

		c.open_trace(&UNITTEST_LEMON_DIAGNOSIS_GUID,flags);

		commit_message message(Context()->Provider,flags);

		LEMON_CHECK(message.want());

		message.write_rawdata(lemon::cbuf("hello the world"));

		message.commit();

		cv.wait(lock);
	}


	LEMON_UNITTEST_CASE(DTraceUnittest,PerformanceOnTest)
	{
		size_t counter = 100000;

		service		S;

		controller  C(S);

		LemonDTraceFlags flags;

		flags.S.Catalog = LEMON_SYS_FILESYSTEM | LEMON_SYS_COROUTINE;

		flags.S.Level = LEMON_TRACELOG_DEBUG;

		C.open_trace(&UNITTEST_LEMON_DIAGNOSIS_GUID,flags);

		provider    P(S,&UNITTEST_LEMON_DIAGNOSIS_GUID);

		for(size_t i = 0; i < counter; ++  i)
		{
			commit_message message(P,flags);

			LEMON_CHECK(message.want());

			message.write_rawdata(lemon::cbuf("hello the world "));
		}	
	}

	LEMON_UNITTEST_CASE(DTraceUnittest,PerformanceOffTest)
	{
		size_t counter = 100000;

		LemonDTraceFlags flags;

		flags.S.Catalog = LEMON_SYS_FILESYSTEM | LEMON_SYS_COROUTINE;

		flags.S.Level = LEMON_TRACELOG_DEBUG;

		//C.open_trace(&UNITTEST_LEMON_DIAGNOSIS_GUID,flags);

		for(size_t i = 0; i < counter; ++  i)
		{
			commit_message message(Context()->Provider,flags);

			LEMON_CHECK(!message.want());

			//message.write(lemon::cbuf("hello the world"));
		}	
	}


	LEMON_UNITTEST_CASE(DTraceUnittest,StructureDataTest)
	{
		LemonDTraceFlags flags;

		flags.S.Catalog = LEMON_SYS_FILESYSTEM | LEMON_SYS_COROUTINE;

		flags.S.Level = LEMON_TRACELOG_DEBUG;

		controller  C(Context()->Service);

		C.open_trace(&UNITTEST_LEMON_DIAGNOSIS_GUID,flags);

		commit_message cm(Context()->Provider,flags);

		cm.write_boolean(true);

		cm.write_boolean(false);

		cm.write_number(12345);

		cm.write_utf8_string("hello world !!!");

		byte_t buffer[] = {0,1,2,3,4,5,6};

		cm.write_userdata(lemon::cbuf(buffer));

		message reader(cm);

		LEMON_CHECK(true == reader.read_boolean());

		LEMON_CHECK(false == reader.read_boolean());

		LEMON_CHECK(12345 == reader.read_integer());

		LEMON_CHECK(reader.read_utf8_string() == "hello world !!!");

		memset(buffer,0,sizeof(buffer));

		LEMON_CHECK(sizeof(buffer) == reader.read_userdata(lemon::buf(buffer)));

		for(size_t i = 0 ; i < sizeof(buffer); ++ i)
		{
			LEMON_CHECK(buffer[i] == i);
		}
	}

	void Hello2(lemon::mutex_t & mutex,condition_variable & cv,const message & e)
	{
		byte_t buffer[7] = {0};

		LEMON_CHECK(LEMON_DTRACE_BOOLEAN == e.type());

		LEMON_CHECK(true == e.read_boolean());

		LEMON_CHECK(LEMON_DTRACE_BOOLEAN == e.type());

		LEMON_CHECK(false == e.read_boolean());

		LEMON_CHECK(LEMON_DTRACE_INTEGER == e.type());

		LEMON_CHECK(12345 == e.read_integer());

		LEMON_CHECK(LEMON_DTRACE_UTF8_STRING == e.type());

		LEMON_CHECK(e.read_utf8_string() == "hello world !!!");

		memset(buffer,0,sizeof(buffer));

		LEMON_CHECK(LEMON_DTRACE_USERDATA == e.type());

		LEMON_CHECK(sizeof(buffer) == e.read_userdata(lemon::buf(buffer)));

		for(size_t i = 0 ; i < sizeof(buffer); ++ i)
		{
			LEMON_CHECK(buffer[i] == i);
		}

		lemon::mutex_t::scope_lock lock(mutex);

		cv.notify();
	}

	LEMON_UNITTEST_CASE(DTraceUnittest,StructureDataTest2)
	{
		lemon::condition_variable cv;

		lemon::mutex_t mutex;

		lemon::mutex_t::scope_lock lock(mutex);

		LemonDTraceFlags flags;

		flags.S.Catalog = LEMON_SYS_FILESYSTEM | LEMON_SYS_COROUTINE;

		flags.S.Level = LEMON_TRACELOG_DEBUG;

		controller  C(Context()->Service);

		C.open_trace(&UNITTEST_LEMON_DIAGNOSIS_GUID,flags);

		consumer cs(C,lemon::bind(&Hello2,lemon::ref(mutex),lemon::ref(cv),lemon::_0));

		commit_message cm(Context()->Provider,flags);

		cm.write_boolean(true);

		cm.write_boolean(false);

		cm.write_number(12345);

		cm.write_utf8_string("hello world !!!");

		byte_t buffer[] = {0,1,2,3,4,5,6};

		cm.write_userdata(lemon::cbuf(buffer));

		cm.commit();

		cv.wait(lock);
	}

	void Hello3(lemon::mutex_t & mutex,condition_variable & cv,const message & e)
	{
		LEMON_CHECK(e.read_utf8_string() == "lemon::dtrace");

		LEMON_CHECK(e.read_integer() == 123);
		
		lemon::mutex_t::scope_lock lock(mutex);

		cv.notify();
	}

	LEMON_UNITTEST_CASE(DTraceUnittest,LemonCxxStandardTraceTest)
	{
		lemon::condition_variable cv;

		lemon::mutex_t mutex;

		lemon::mutex_t::scope_lock lock(mutex);

		LemonDTraceFlags flags;

		flags.S.Catalog = LEMON_SYS_FILESYSTEM | LEMON_SYS_COROUTINE;

		flags.S.Level = LEMON_TRACELOG_DEBUG;

		controller  C(Context()->Service);

		C.open_trace(&UNITTEST_LEMON_DIAGNOSIS_GUID,flags);

		consumer cs(C,lemon::bind(&Hello3,lemon::ref(mutex),lemon::ref(cv),lemon::_0));

		LEMON_SYS_TRACE(Context()->Provider,LEMON_TRACELOG_DEBUG,LEMON_SYS_FILESYSTEM,"Hello world ,{0},{1}" ,"lemon::dtrace",123);

		cv.wait(lock);
	}

}}}
