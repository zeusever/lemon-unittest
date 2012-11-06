#include <hash_map>
#include <lemon/io/irp.h>
#include <lemon/io/assembly.h>
#include <lemonxx/sys/sys.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <lemon/memory/fixobj.h>

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
#ifdef LEMON_IO_IOCP
	lemon_bool CancelIRProc(LemonIRP /*irp*/,const LemonErrorInfo * errorCode)
#else
	lemon_bool CancelIRProc(LemonIRP /*irp*/,LemonErrorInfo * errorCode)
#endif 
	{
		LEMON_CHECK(LEMON_ERRORINOF_EQ(errorCode->Error,LEMON_IO_ASYNC_CANCEL) == lemon_true);

		return lemon_true;
	}

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

			irp->Proc = &CancelIRProc;

			LemonInsertIRP_TS(table,(__lemon_io_file)623,irp,errorCode);
		}

		LEMON_CHECK(LemonIRPCounter_TS(table,(__lemon_io_file)623) == counter);

		LEMON_CHECK(LemonIRPCounter_TS(table,(__lemon_io_file)(counter + 10)) == 0);

		LemonIRPFileCancel_TS(table,(__lemon_io_file)623);

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

			irp->Proc = &CancelIRProc;

			LemonInsertIRP_TS(table,(__lemon_io_file)i,irp,errorCode);
		}

		LemonCloseIRPTable_TS(table);
	}

	LEMON_UNITTEST_CASE(LemonIRPTableUnittest,TableCancelTest)
	{
		lemon::error_info errorCode;

		LemonIRPTable table = LemonCreateIRPTable_TS(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(table));

		size_t counter = 100;

		for(size_t i = 0; i < counter; ++ i)
		{
			for(size_t i = 0; i < 20000; ++i)
			{
				LemonIRP irp = LemonCreateIRP_TS(table,errorCode);

				irp->Proc = &CancelIRProc;

				LemonInsertIRP_TS(table,(__lemon_io_file)i,irp,errorCode);
			}

			LemonIRPTableCancel_TS(table);
		}

		LemonCloseIRPTable_TS(table);
	}

	LEMON_UNITTEST_CASE(LemonIRPTableUnittest,FileCancelTest)
	{
		lemon::error_info errorCode;

		LemonIRPTable table = LemonCreateIRPTable_TS(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(table));

		size_t counter = 2000;

		for(size_t i = 0; i < counter; ++ i)
		{
			for(size_t i = 0; i < 100; ++i)
			{
				LemonIRP irp = LemonCreateIRP_TS(table,errorCode);

				irp->Proc = &CancelIRProc;

				LemonInsertIRP_TS(table,(__lemon_io_file)100,irp,errorCode);
			}

			//LemonIRPFileCancel_TS(table,(__lemon_io_file)100);

			LemonIRPCloseFile_TS(table,(__lemon_io_file)100);
		}

		LemonCloseIRPTable_TS(table);
	}

#ifdef LEMON_IO_IOCP

	lemon_bool IRProc1(LemonIRP irp,const LemonErrorInfo * /*errorCode*/)
	{
		size_t &counter = *(size_t*)irp->UserData;

		++ counter;

		return lemon_false;
	}

	LEMON_UNITTEST_CASE(LemonIRPTableUnittest,TableCompleteTest)
	{
		lemon::error_info errorCode;

		LemonIRPTable table = LemonCreateIRPTable_TS(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(table));

		LemonFixObjectAllocator allocator = LemonCreateFixObjectAllocator(LEMON_HANDLE_IMPLEMENT_SIZEOF(LemonIO),errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_USER_ERROR(errorCode,LEMON_IO_ASYNC_CANCEL);

		size_t counter = 2000;

		for(size_t i = 0; i < counter; ++ i)
		{
			for(size_t i = 0; i < 100; ++i)
			{
				LemonIRP irp = LemonCreateIRP_TS(table,errorCode);

				irp->Proc = &CancelIRProc;

				LemonIO io = (LemonIO)LemonFixObjectAlloc(allocator);

				io->Handle = (__lemon_io_file)i;

				irp->Self = io;

				LemonInsertIRP_TS(table,(__lemon_io_file)i,irp,errorCode);

				LemonIRPComplete_TS(table,irp,&errorCode);
			}

			LEMON_CHECK(LemonIRPTableSize(table) == 100);

			LemoFixObjectFreeAll(allocator);
		}


		LemonIRPTableCancel_TS(table);

		size_t n = 0;

		LemonIRP irp = LemonCreateIRP_TS(table,errorCode);

		irp->Proc = &IRProc1;

		LemonIO io = (LemonIO)LemonFixObjectAlloc(allocator);

		io->Handle = (__lemon_io_file)1024;

		irp->Self = io;

		irp->UserData = &n;

		LemonInsertIRP_TS(table,(__lemon_io_file)1024,irp,errorCode);

		LEMON_CHECK(LemonIRPTableSize(table) == 1);

		LemonIRPFileCancel_TS(table,(__lemon_io_file)1024);

		LEMON_CHECK(LemonIRPTableSize(table) == 1);

		LemonIRPComplete_TS(table,irp,&errorCode);

		LEMON_CHECK(LemonIRPTableSize(table) == 1);

		LemonIRPCloseFile_TS(table,(__lemon_io_file)1024);

		LEMON_CHECK(LemonIRPTableSize(table) == 0);

		assert(n == 1);

		LemonReleaseFixObjectAllocator(allocator);

		LemonCloseIRPTable_TS(table);

	}
#else

	lemon_bool IRProc1(LemonIRP /*irp*/,LemonErrorInfo * /*errorCode*/)
	{
		return lemon_false;
	}

	void IRPCompleteF(void * /*userdata*/,LemonIRP /*irp*/,const LemonErrorInfo * /*errorCode*/)
	{
		
	}

	lemon_bool IOStatusF(void * userdata,__lemon_io_file /*handle*/)
	{
		++ *((size_t*)userdata);

		return lemon_true;
	}

	LEMON_UNITTEST_CASE(LemonIRPTableUnittest,ExecuteIRPsTest)
	{
		lemon::error_info errorCode;

		LemonIRPTable table = LemonCreateIRPTable_TS(errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(LEMON_CHECK_HANDLE(table));

		size_t counter = 2000;

		LEMON_USER_ERROR(errorCode,LEMON_IO_ASYNC_CANCEL);

		for(size_t i = 0; i < counter; ++i)
		{
			LemonIRP irp = LemonCreateIRP_TS(table,errorCode);

			irp->Proc = &CancelIRProc;

			LemonInsertIRP_TS(table,(__lemon_io_file)i,irp,errorCode);
		}

		LEMON_CHECK(LemonIRPTableSize(table) == counter);

		LEMON_CHECK(LemonIRPCounter_TS(table,(__lemon_io_file)623) == 1);

		LemonExecuteIRPs_TS(table,(__lemon_io_file)623,&IRPCompleteF,NULL);

		LEMON_CHECK(LemonIRPCounter_TS(table,(__lemon_io_file)623) == 0);

		LEMON_CHECK(LemonIRPTableSize(table) == counter);

		LemonIRPCloseFile_TS(table,(__lemon_io_file)623);

		LEMON_CHECK(LemonIRPTableSize(table) == counter - 1);

		size_t n = 0;

		LemonExecuteIRPsEx_TS(table,&IOStatusF,&n,&IRPCompleteF,NULL);

		LEMON_CHECK(LemonIRPTableSize(table) == counter - 1);

		LemonIRPTableCancel_TS(table);

		LEMON_CHECK(LemonIRPTableSize(table) == 0);

		LEMON_CHECK(n == counter - 1);

		LemonCloseIRPTable_TS(table);
	}
#endif 

}}}
