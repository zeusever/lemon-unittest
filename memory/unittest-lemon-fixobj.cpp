#include <iostream>
#include <lemonxx/sys/sys.hpp>
#include <lemonxx/memory/fixobj.hpp>
#include <lemonxx/memory/smallobj.hpp>
#include <lemonxx/memory/pool.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace memory{namespace test{

	struct LemonFixObjectAllocatorUnittest{};

	template<size_t BlockSize,size_t MaxLoops = 100>
	struct MemoryAllocUnittest
	{
	public: 
		void Run()
		{
			MemoryPoolTest();

			SmallObjAllocatorTest();

			FixAllocatorTest();

			NewTest();

			MallocFreeTest();
		}
	private:
		void MemoryPoolTest()
		{
			pool::allocator<1048576> allocator;

			timer_t t;

			for(size_t i = 0 ; i < MaxLoops; ++ i)
			{
				_buffer[i] = (byte_t*)allocator.alloc(BlockSize);
			}

			time_duration d = t.duration();

			std::cout << "pool::allocator alloc " << MaxLoops << " times:" << d << std::endl;

			t.reset();

			allocator.garbagecollect();

			d = t.duration();

			std::cout << "pool::allocator free " << MaxLoops << " times:" << d << std::endl;
		}

		void SmallObjAllocatorTest()
		{
			smallobject::allocator<256> allocator;

			timer_t t;

			for(size_t i = 0 ; i < MaxLoops; ++ i)
			{
				_buffer[i] = (byte_t*)allocator.alloc(BlockSize);
			}

			time_duration d = t.duration();

			std::cout << "smallobject::allocator alloc " << MaxLoops << " times:" << d << std::endl;

			t.reset();

			for(size_t i = 0 ; i < MaxLoops; ++ i)
			{
				allocator.free(_buffer[i],BlockSize);
			}

			d = t.duration();

			std::cout << "smallobject::allocator free " << MaxLoops << " times:" << d << std::endl;
		}

		void FixAllocatorTest()
		{
			fixed::allocator<BlockSize> allocator;

			timer_t t;

			for(size_t i = 0 ; i < MaxLoops; ++ i)
			{
				_buffer[i] = (byte_t*)allocator.alloc();
			}

			time_duration d = t.duration();

			std::cout << "lemon::fixobject_allocator alloc " << MaxLoops << " times:" << d << std::endl;

			t.reset();

			for(size_t i = 0 ; i < MaxLoops; ++ i)
			{
				allocator.free(_buffer[i]);
			}

			d = t.duration();

			std::cout << "lemon::fixobject_allocator free " << MaxLoops << " times:" << d << std::endl;
		}

		void NewTest()
		{
			timer_t t;

			for(size_t i = 0 ; i < MaxLoops; ++ i)
			{
				_buffer[i] = new byte_t[BlockSize];
			}

			time_duration d = t.duration();

			std::cout << "global new " << MaxLoops << " times:" << d << std::endl;

			t.reset();

			for(size_t i = 0 ; i < MaxLoops; ++ i)
			{
				delete[] _buffer[i];
			}

			d = t.duration();

			std::cout << "global delete " << MaxLoops << " times:" << d << std::endl;
		}

		void MallocFreeTest()
		{
			timer_t t;

			for(size_t i = 0 ; i < MaxLoops; ++ i)
			{
				_buffer[i] = (byte_t *)malloc(BlockSize);
			}

			time_duration d = t.duration();

			std::cout << "malloc " << MaxLoops << " times:" << d << std::endl;

			t.reset();

			for(size_t i = 0 ; i < MaxLoops; ++ i)
			{
				free(_buffer[i]);
			}

			d = t.duration();

			std::cout << "free " << MaxLoops << " times:" << d << std::endl;
		}

	private:

		byte_t * _buffer[MaxLoops];
	};

	LEMON_UNITTEST_CASE(LemonFixObjectAllocatorUnittest,Byte8AllocTest)
	{
		MemoryAllocUnittest<8>().Run();
	}


	LEMON_UNITTEST_CASE(LemonFixObjectAllocatorUnittest,Byte16AllocTest)
	{
		MemoryAllocUnittest<16>().Run();
	}

	LEMON_UNITTEST_CASE(LemonFixObjectAllocatorUnittest,Byte32AllocTest)
	{
		MemoryAllocUnittest<32>().Run();
	}

	LEMON_UNITTEST_CASE(LemonFixObjectAllocatorUnittest,Byte64AllocTest)
	{
		MemoryAllocUnittest<64>().Run();
	}

	LEMON_UNITTEST_CASE(LemonFixObjectAllocatorUnittest,Byte128AllocTest)
	{
		MemoryAllocUnittest<128>().Run();
	}

	LEMON_UNITTEST_CASE(LemonFixObjectAllocatorUnittest,Byte256AllocTest)
	{
		MemoryAllocUnittest<256>().Run();
	}

}}}