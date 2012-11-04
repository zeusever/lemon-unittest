#include <lemon/io/irp.h>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{

	class FIFOQueueUnittest{};

	LEMON_UNITTEST_CASE(FIFOQueueUnittest,PushRemoveTest)
	{
		lemon::error_info errorCode;

		LemonIRPTable table = LemonCreateIRPTable_TS(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(table));

		LemonIRPTableFileObj obj  = LemonCreateIRPTableFileObj(table,NULL,(__lemon_io_file)100,errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(obj));

		LemonIRP header,tail,irp1,irp2;

		header = LemonCreateIRP_TS(table,errorCode);

		irp1 = LemonCreateIRP_TS(table,errorCode);

		irp2 = LemonCreateIRP_TS(table,errorCode);

		tail = LemonCreateIRP_TS(table,errorCode);

		LEMON_CHECK(LemonIRPCounter(obj) == 0);

		LemonPushIRP(obj,header);

		LEMON_CHECK(LemonIRPCounter(obj) == 1);

		LemonPushIRP(obj,irp1);

		LEMON_CHECK(LemonIRPCounter(obj) == 2);

		LemonPushIRP(obj,irp2);

		LEMON_CHECK(LemonIRPCounter(obj) == 3);

		LemonPushIRP(obj,tail);

		LEMON_CHECK(LemonIRPCounter(obj) == 4);

		LemonRemoveIRP(obj,irp1);

		LEMON_CHECK(LemonIRPCounter(obj) == 3);

		LemonRemoveIRP(obj,header);

		LEMON_CHECK(LemonIRPCounter(obj) == 2);

		LemonRemoveIRP(obj,irp2);

		LEMON_CHECK(LemonIRPCounter(obj) == 1);

		LemonRemoveIRP(obj,tail);

		LEMON_CHECK(LemonIRPCounter(obj) == 0);

		LemonCloseIRPTable_TS(table);

	}

	LEMON_UNITTEST_CASE(FIFOQueueUnittest,RemoveAllCloseTest)
	{
		lemon::error_info errorCode;

		LemonIRPTable table = LemonCreateIRPTable_TS(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(table));

		LemonIRPTableFileObj obj  = LemonCreateIRPTableFileObj(table,NULL,(__lemon_io_file)100,errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(obj));

		LemonIRP other = NULL;

		size_t counter = 100000;

		for(size_t i = 0 ; i < counter; ++ i)
		{
			LemonIRP irp = LemonCreateIRP_TS(table,errorCode);

			if(i == 40000) other = irp;

			LemonPushIRP(obj,irp);
		}

		LemonRemoveIRP(obj,other);

		LEMON_CHECK(LemonIRPCounter(obj) == counter - 1);

		LemonCloseIRPs_TS(table,LemonRemoveIRPsAll(obj));

		LEMON_CHECK(LemonIRPCounter(obj) == 0);

		LemonCloseIRPTable_TS(table);
	}

}}}
