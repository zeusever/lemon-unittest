#include <lemon/io/reactor_events.h>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{

	class LemonIOEventsUnittest{};

	lemon_bool TestIOExecutor(LemonIOEvent /*E*/, size_t * /*numberOfBytesTransferred*/, LemonErrorInfo * errorCode)
	{
		LEMON_RESET_ERRORINFO(*errorCode);

		return lemon_true;
	}

	void CallBack(void * userdata , size_t /*numberOfBytesTransferred*/ , const LemonErrorInfo * /*errorCode*/)
	{
		++ *(size_t*)userdata;
	}

	LEMON_UNITTEST_CASE(LemonIOEventsUnittest,LemonIOEventQCreateTest)
	{
		lemon::error_info errorCode;

		LemonIOEventQ Q = LemonCreateIOEventQ(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(Q != NULL);

		LemonIOEventCompleteQ completeQ = LemonCreateIOEventCompleteQ(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(completeQ != NULL);

		size_t maxCounter = 10000;

		size_t counter = 0;

		for(size_t i = 0; i < maxCounter; ++ i){

			LemonIOEvent E = LemonCreateIOEvent(Q,errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			LEMON_CHECK(LEMON_CHECK_HANDLE(E));

			E->UserData = &counter;

			E->CallBack.RW = &CallBack;

			LemonRegisterIOEvent(Q,(__lemon_io_file)i,E,errorCode);
		}

		for(size_t i = 0; i < 100; ++ i)
		{
			counter = 0;

			for(size_t i = 0; i < maxCounter; ++ i){

				LemonIOEvent E = LemonCreateIOEvent(Q,errorCode);

				LEMON_CHECK(LEMON_SUCCESS(errorCode));

				LEMON_CHECK(LEMON_CHECK_HANDLE(E));

				E->UserData = &counter;

				E->CallBack.RW = &CallBack;

				LemonRegisterIOEvent(Q,(__lemon_io_file)(maxCounter + 1),E,errorCode);

				LEMON_CHECK(LEMON_SUCCESS(errorCode));
			}

			LEMON_CHECK(maxCounter == LemonIOEventsOfFile(Q,(__lemon_io_file)(maxCounter + 1)));

			LemonExecuteIOEvents(Q,completeQ,(__lemon_io_file)(maxCounter + 1),&TestIOExecutor,errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			LemonCompletedIOEvent completeIOEvent = {0};

			while(LemonQueryCompleteIO(completeQ,&completeIOEvent,0,errorCode) == lemon_true)
			{
				LEMON_CHECK(LEMON_SUCCESS(errorCode));

				LEMON_CHECK(completeIOEvent.CallBack.RW == &CallBack);

				completeIOEvent.CallBack.RW(completeIOEvent.UserData,completeIOEvent.NumberOfBytesTransferred,&completeIOEvent.ErrorCode);
			}

			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			LEMON_CHECK(maxCounter == counter);

		}

		LEMON_CHECK(Q->Counter == maxCounter);

		LemonReleaseIOEventCompleteQ(completeQ);

		LemonReleaseIOEventQ(Q);
	}

	LEMON_UNITTEST_CASE(LemonIOEventsUnittest,LemonQueryCompleteIOTest)
	{
		lemon::error_info errorCode;

		LemonIOEventQ Q = LemonCreateIOEventQ(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(Q != NULL);

		LemonIOEventCompleteQ completeQ = LemonCreateIOEventCompleteQ(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(completeQ != NULL);

		LemonIOEvent E = LemonCreateIOEvent(Q,errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(E));

		E->CallBack.RW = &CallBack;

		LemonRegisterIOEvent(Q,(__lemon_io_file)100,E,errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LemonExecuteIOEvents(Q,completeQ,(__lemon_io_file)100,&TestIOExecutor,errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LemonCompletedIOEvent completeIOEvent = {0};

		LEMON_CHECK(lemon_true == LemonQueryCompleteIO(completeQ,&completeIOEvent,0,errorCode));

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(completeIOEvent.CallBack.RW == &CallBack);

		LEMON_CHECK(lemon_false == LemonQueryCompleteIO(completeQ,&completeIOEvent,1000,errorCode));

		LemonReleaseIOEventCompleteQ(completeQ);

		LemonReleaseIOEventQ(Q);
	}
}}}
