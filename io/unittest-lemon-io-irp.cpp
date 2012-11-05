#include <hash_map>
#include <lemon/io/irp.h>
#include <lemonxx/sys/sys.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{

	class FIFOQueueUnittest{};

	LEMON_UNITTEST_CASE(FIFOQueueUnittest,PushRemoveTest)
	{
		lemon::error_info errorCode;

		LemonIRPTable table = LemonCreateIRPTable_TS(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(table));

		LemonIRPTableFileObj obj  = LemonCreateIRPTableFileObj(table,(__lemon_io_file)100,errorCode);

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

		LemonIRPTableFileObj obj  = LemonCreateIRPTableFileObj(table,(__lemon_io_file)100,errorCode);

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

	class LemonIRPTableUnittest{};

	LEMON_UNITTEST_CASE(LemonIRPTableUnittest,LemonIRPTableFileObjTest)
	{
		lemon::error_info errorCode;

		LemonIRPTable table = LemonCreateIRPTable_TS(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(table));

		size_t counter = 10000;

		for(size_t i = 0; i < counter; ++i)
		{
			LemonIRPTableFileObj obj  = LemonCreateIRPTableFileObj(table,(__lemon_io_file)i,errorCode);

			LemonIRPTableAdd(table,obj,errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));
		}

		lemon::random_t random;

		for(size_t i = 0; i < counter; ++i)
		{
			LEMON_CHECK(LEMON_CHECK_HANDLE(LemonIRPTableSearch(table,(__lemon_io_file)(random.next() % counter))));
		}
		LEMON_CHECK(LemonIRPTableSize(table) == counter);

		LemonIRPTableFileObj obj = LemonIRPTableSearch(table,(__lemon_io_file)655);

		LEMON_CHECK(LEMON_CHECK_HANDLE(obj));

		LemonIRPTableRemove(table,obj);

		LEMON_CHECK(LemonIRPTableSize(table) == counter - 1);

		LEMON_CHECK(LemonIRPTableLoadFactor(table) <= LEMON_IO_HASHMAP_LOAD_FACTOR);

		LemonCloseIRPTable_TS(table);
	}

	LEMON_UNITTEST_CASE(LemonIRPTableUnittest,HashFunctionTest)
	{
		for(size_t i = 0; i < 100000000; ++ i)
		{
			LemonIRPTableHashF(1024,(__lemon_io_file)i);
		}
	}

	LEMON_UNITTEST_CASE(LemonIRPTableUnittest,TableInsertRemoveTest)
	{
		lemon::error_info errorCode;

		LemonIRPTable table = LemonCreateIRPTable_TS(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(table));

		size_t counter = 1000000;

		for(size_t i = 0; i < counter; ++i)
		{
			LemonIRPTableFileObj obj  = LemonCreateIRPTableFileObj(table,(__lemon_io_file)i,errorCode);

			LemonIRPTableAdd(table,obj,errorCode);

			LEMON_CHECK(LEMON_SUCCESS(errorCode));
		}

		for(size_t i = 0; i < counter; ++i)
		{
			LemonIRP irp = LemonCreateIRP_TS(table,errorCode);

			LemonInsertIRP_TS(table,(__lemon_io_file)623,irp,errorCode);
		}

		LEMON_CHECK(LemonIRPCounter_TS(table,(__lemon_io_file)623) == counter);

		LEMON_CHECK(LemonIRPCounter_TS(table,(__lemon_io_file)(counter + 10)) == 0);

		LemonRemoveIRPsAll_TS(table,(__lemon_io_file)623);

		LEMON_CHECK(LemonIRPCounter_TS(table,(__lemon_io_file)623) == 0);

		LemonCloseIRPTable_TS(table);
	}

	LEMON_UNITTEST_CASE(LemonIRPTableUnittest,TableInsertPerformanceTest)
	{
		lemon::error_info errorCode;

		LemonIRPTable table = LemonCreateIRPTable_TS(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(table));

		size_t counter = 2000000;

		for(size_t i = 0; i < counter; ++i)
		{
			LemonIRP irp = LemonCreateIRP_TS(table,errorCode);

			LemonInsertIRP_TS(table,(__lemon_io_file)i,irp,errorCode);
		}

		LemonCloseIRPTable_TS(table);
	}

	LEMON_UNITTEST_CASE(LemonIRPTableUnittest,STDHashTablePerformanceTest)
	{
		std::hash_map<size_t,int> hashTable;

		size_t counter = 10000;

		for(size_t i = 0; i < counter; ++i)
		{
			hashTable.insert(std::make_pair(i,1));
		}

		lemon::random_t random;

		lemon::timer_t clock;

		for(size_t i = 0; i < counter; ++i)
		{
			hashTable[random.next() % counter];
		}

		time_duration duration = clock.duration();

		std::cout << "search (" << duration / 10000000 << "." 

			<< std::setw(6) << std::setfill('0') <<(duration % 10000000) / 10 

			<< " s)" << std::endl;
	}
}}}
