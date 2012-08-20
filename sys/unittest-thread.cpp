#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/sys/thread.hpp>
#include <lemonxx/function/bind.hpp>

namespace lemon{namespace test{
	
	struct LemonThreadUnittest{};

	void Proc(int i)
	{
		LEMON_CHECK(i == 1);
	}

	LEMON_UNITTEST_CASE(LemonThreadUnittest,ThreadCreateTest)
	{
		

		/// <summary> create the empty thread object,you must call start() to create real thread.  </summary>
		thread_t t;

		LEMON_CHECK(t.empty());

		t.start(bind(&Proc,1));

		LEMON_CHECK(!t.empty());

		t.join();
	}

	class TlsCounter : private nocopyable
	{
	public:

		TlsCounter(atomic_t & i):_i(i){ ++ i;}

		~TlsCounter(){ -- _i;}

		atomic_t Data(){return _i;}

	private:

		atomic_t &_i;
	};

	typedef lemon::tlsptr<TlsCounter> TlsCounterPtr;

	struct TlsContext
	{
		atomic_t		i;

		TlsCounterPtr	ptr;
	};

	void TlsDestructorPoc(void * data)
	{
		TlsContext * context = (TlsContext*)data;

		LEMON_CHECK(0 == context->i);

		LEMON_CHECK(context->ptr.empty());

		context->ptr.reset(new TlsCounter(context->i));

		LEMON_CHECK(1 == context->i);
	}

	LEMON_UNITTEST_CASE(LemonThreadUnittest,TlsDestructorTest)
	{
		TlsContext context;

		context.i = 0;

		for(size_t i = 0; i < 10; ++ i)
		{
			error_info errorCode;

			LemonThread t = LemonCreateThread(&TlsDestructorPoc,&context,&errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			LEMON_CHECK(LEMON_CHECK_HANDLE(t));

			LemonThreadJoin(t,&errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			LemonReleaseThread(t);
		}

		//LEMON_CHECK(0 == context.i);
	}

	/*LEMON_UNITTEST_CASE(LemonThreadUnittest,ThreadTextEncodingTest)
	{
		error_info errorCode;

		LemonTextEncoding ec = LemonGetThreadTextEncoding(&errorCode);

		LEMON_CHECK(LEMON_CHECK_HANDLE(ec));

		LemonTextEncoding ec1 = LemonGetThreadTextEncoding(&errorCode);

		LEMON_CHECK(LEMON_CHECK_HANDLE(ec1));

		LEMON_CHECK(ec1 == ec);
	}*/

	struct MutexContext
	{
		size_t				I;

		lemon::mutex	Mutex;
	};

	void MutexTestProc(void * data)
	{
		MutexContext * context = (MutexContext *)data;

		lemon::mutex::scope_lock lock(context->Mutex);

		++ context->I;
	}

	LEMON_UNITTEST_CASE(LemonThreadUnittest,MutexTest)
	{
		MutexContext context;

		context.I = 0;

		const size_t maxThreads = 100;

		LemonThread threads[maxThreads];

		for(size_t i = 0; i < maxThreads; ++ i)
		{
			error_info errorCode;

			threads[i] = LemonCreateThread(&MutexTestProc,&context,&errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			LEMON_CHECK(LEMON_CHECK_HANDLE(threads[i]));
		}

		for(size_t i = 0; i < maxThreads; ++ i)
		{
			error_info errorCode;

			LemonThreadJoin(threads[i],&errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			LemonReleaseThread(threads[i]);
		}

		LEMON_CHECK(maxThreads == context.I);
	}

	void AtomicOperator(void * data)
	{
		atomic_t * i = reinterpret_cast<atomic_t*>(data);

		++ (*i);

		(*i) -- ;		

		(*i) ++ ;

		-- (*i);

	}

	LEMON_UNITTEST_CASE(LemonThreadUnittest,AtomicTest)
	{
		atomic_t atomic = 0;

		const size_t maxThreads = 100;

		LemonThread threads[maxThreads];

		for(size_t i = 0; i < maxThreads; ++ i)
		{
			error_info errorCode;

			threads[i] = LemonCreateThread(&AtomicOperator,&atomic,&errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			LEMON_CHECK(LEMON_CHECK_HANDLE(threads[i]));
		}

		for(size_t i = 0; i < maxThreads; ++ i)
		{
			error_info errorCode;

			LemonThreadJoin(threads[i],&errorCode);

			LemonThreadJoin(threads[i],&errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			LemonReleaseThread(threads[i]);
		}

		LEMON_CHECK(atomic == 0);
	}

	struct NofiyContext{
		condition_variable cv;

		mutex m;

		bool entry;
	};

	void Notify(void * data)
	{
		NofiyContext * context = (NofiyContext*)data;

		mutex::scope_lock lock(context->m);

		context->entry = true;

		context->cv.notify();

		context->cv.wait(lock);
	}

	LEMON_UNITTEST_CASE(LemonThreadUnittest,ConditionNotifyTest)
	{
		NofiyContext context;

		context.entry = false;

		error_info errorCode;

		LemonThread t  = LemonCreateThread(&Notify,&context,&errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		{
			mutex::scope_lock lock(context.m);

			while(!context.entry) context.cv.wait(lock);
		}
		context.cv.notify();

		LemonThreadJoin(t,&errorCode);

		LemonReleaseThread(t);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));
	}

	void Increase(atomic_t & i)
	{
		++ i;
	}

	LEMON_UNITTEST_CASE(LemonThreadUnittest,thread_group_test)
	{
		atomic_t i;

		thread_group group;

		group.start(lemon::bind(&Increase,ref(i)),20);

		group.join();

		LEMON_CHECK(group.size() == (size_t)i);
	}

}}