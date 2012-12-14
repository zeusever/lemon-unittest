#include <lemonxx/function/bind.hpp>
#include <lemonxx/runQ/runQ.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace runQ{namespace test{

	class RunQUnittest{};

	struct exit_job
	{
		exit_job(runQ_service & service,job_id)
		{
			service.stop();
		}

		void recv(runQ_service & /*service*/,job_id,job_id /*source*/,const_buffer /*buffer*/)
		{
			LEMON_CHECK(false && "can't here");
		}
	};
	
	

	LEMON_UNITTEST_CASE(RunQUnittest,LoopTest)
	{
		runQ_service service;

		basic_job_class<exit_job> job(service);

		service.run();
	}

	struct faild_route_job
	{
		faild_route_job(runQ_service & service,job_id self)
		{
			LEMON_UNITTEST_EXPECT_EXCEPTION(service.send(self,LEMON_MAKE_JOB_ID(1,1),mutable_buffer()),error_info);

			service.stop();
		}

		void recv(runQ_service & /*service*/,job_id,job_id /*source*/,const_buffer /*buffer*/)
		{
			LEMON_CHECK(false && "can't here");
		}
	};

	LEMON_UNITTEST_CASE(RunQUnittest,RouteRemoteFailedTest)
	{
		runQ_service service;

		basic_job_class<faild_route_job> job(service);

		service.run();
	}

	struct exit_message_job
	{
		static const int counter = 100000;

		exit_message_job(runQ_service & service,job_id id) 
		{
			mutable_buffer buffer = service.alloc(sizeof(size_t));

			buffer_cast<size_t>(buffer) = counter;

			service.send(id,id,buffer);
		}

		void recv(runQ_service & service,job_id self,job_id source,mutable_buffer buffer)
		{
			LEMON_CHECK(source == self);

			if(0 == buffer_cast<size_t>(buffer))
			{
				service.stop();
			}
			else
			{
				-- buffer_cast<size_t>(buffer);

				service.send(self,source,buffer);
			}
		}
	};

	LEMON_UNITTEST_CASE(RunQUnittest,SendTest)
	{
		runQ_service service;

		basic_job_class<exit_message_job> job(service);

		thread_group works(lemon::bind(&runQ_service::run,&service),1);

		works.join();

		//service.run();
	}

	struct MapMessage
	{
		int				Type;

		union{
			
			byte_t		*WorkData; 

			job_id		ReduceJob;

		}				Data;
	};

	class WorkJob
	{
	public:
		WorkJob(runQ_service &,job_id ){}

		void recv(runQ_service & service,job_id self,job_id /*source*/,const_buffer buffer)
		{
			if(buffer_cast<MapMessage>(buffer).Type == 0)
			{
				Data = buffer_cast<MapMessage>(buffer).Data.WorkData;

				service.free(buffer);
			}
			else
			{
				++ *Data;

				lemon::sleep(10);//simulate io operation

				service.send(self,buffer_cast<MapMessage>(buffer).Data.ReduceJob,mutable_buffer());

				service.free(buffer);

				service.close_job(self);
			}
		}

	private:
		byte_t												*Data;
	};

	class MapJob
	{
	public:

		const static int maxcounter = 3000;

		MapJob(runQ_service & service,job_id self):counter(0),_buffer(maxcounter,0),_jobs(maxcounter,0)
		{

			for(size_t i = 0; i < maxcounter; ++ i)
			{
				_jobs[i] = basic_job_class<WorkJob>::create(service);

				MapMessage & message = buffer_cast<MapMessage>(service.alloc(sizeof(MapMessage)));

				message.Data.WorkData = &_buffer[i];

				service.send(self,_jobs[i],buf(message));
			}

			service.send(self,self,service.alloc(sizeof(size_t)));
		}

		void recv(runQ_service & service,job_id self,job_id /*source*/,mutable_buffer buffer)
		{
			if(!buffer.empty())
			{
				for(size_t i = 0; i < maxcounter; ++ i)
				{
					MapMessage & message = buffer_cast<MapMessage>(service.alloc(sizeof(MapMessage)));

					message.Type = 1;

					message.Data.ReduceJob = self;

					service.send(self,_jobs[i],buf(message));
				}
			}
			else
			{
				++ counter;

				if(counter == maxcounter) 
				{
					for(size_t i = 0; i < maxcounter; ++ i)
					{
						LEMON_CHECK(_buffer[i] == 1);
					}

					service.stop();
				}
			}
		}

	private:
		size_t													counter;

		std::vector<byte_t>										_buffer;

		std::vector<job_id>										_jobs;
	};


	LEMON_UNITTEST_CASE(RunQUnittest,PerformanceThreadOneTest)
	{
		runQ_service service;

		basic_job_class<MapJob> job(service);

		service.run();
	}

	LEMON_UNITTEST_CASE(RunQUnittest,PerformanceThreadTwoTest)
	{
		runQ_service service;

		basic_job_class<MapJob> job(service);

		thread_group works(lemon::bind(&runQ_service::run,&service),2);

		works.join();
	}

	LEMON_UNITTEST_CASE(RunQUnittest,PerformanceThreadFourTest)
	{
		runQ_service service;

		basic_job_class<MapJob> job(service);

		thread_group works(lemon::bind(&runQ_service::run,&service),4);

		works.join();
	}

}}}
