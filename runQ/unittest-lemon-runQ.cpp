#include <lemonxx/function/bind.hpp>
#include <lemon/runQ/assembly.h>
#include <lemonxx/runQ/runQ.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace runQ{namespace test{

	class RunQUnittest{};

	class ExitJob : public basic_job_class<ExitJob>
	{
	public:
		void initialize(void*)
		{
			close();
		}

		void uninitialize()
		{
			exit();
		}
	};

	 LEMON_UNITTEST_CASE(RunQUnittest,ExitJobTest)
	 {
	 	runQ_service Q;

	 	ExitJob::create(Q);

		thread_group works(lemon::bind(&runQ_service::run,&Q),8);

		works.join();
	 }

	 class GroupJob : public basic_job_class<GroupJob>
	 {
	 public:

		 void initialize(void* param)
		 {
			 _group = *(job_id*)param;

			 entry_group(_group);
		 }

		 void multicast(lemon_job_id, const_buffer buffer)
		 {
			 LEMON_CHECK(!LEMON_CHECK_BUFF(buffer));

			 leave_group(_group);

			 close();
		 }

	 private:

		 job_id _group; 
	 };

	  class GroupServer : public basic_job_class<GroupServer>
	  {
	  public:

		  static const size_t counter = 100000;

		  void initialize(void* param)
		  {
			  _group = *(job_id*)param;

			  for(size_t i = 0; i < counter; ++ i)
			  {
				  GroupJob::create(service(),&_group);
			  }

			  start_timer(1000);
		  }

		  void timeout()
		  {
			  lemon::timer_t timer;

			  send(_group,mutable_buffer());

			  time_duration duration = timer.duration();

			  std::cout << "multicast send(" << counter << ") -- success(" 
				  
				  << duration / 10000000 << "." 

				  << std::setw(6) << std::setfill('0') <<(duration % 10000000) / 10 

				  << " s)" << std::endl;

			  std::cout << "group job counter(" << jobs(service()) << ")" << std::endl;

			  if(jobs(service()) == 1)  exit();
			 
		  }


	  private:

		  job_id _group; 
	  };



	 LEMON_UNITTEST_CASE(RunQUnittest,GroupTest)
	 {
		 runQ_service Q;

		 job_id group = create_group(Q);

		 LEMON_CHECK(LEMON_JOBID_IS_MULTICAST(group));

		 GroupServer::create(Q,&group);

		 Q.run();
	 }

	class ResetJob : public basic_job_class<ResetJob>
	{
	public:
		void recv(lemon_job_id, mutable_buffer /*buff*/)
		{
			exit();
		}
	};

	 LEMON_UNITTEST_CASE(RunQUnittest,ResetJobTest)
	 {
	 	runQ_service Q;

	 	for(size_t i = 0; i < 10000; ++ i)
	 	{
	 		job_id  id = ResetJob::create(Q);

	 		runQ::send(Q,LEMON_INVALID_JOBID,id,mutable_buffer());

	 		Q.run();
	
	 		Q.reset();

	 		LEMON_CHECK(Q.jobs() == 0);
	 	}
	 }

	class TimeoutJob : public basic_job_class<TimeoutJob>
	{
	public:
		TimeoutJob():_counter(0){}

		void initialize(void*)
		{
			start_timer(100);
		}

		void timeout()
		{
			++ _counter;

			std::cout << "TimeoutJob called -- (" << _counter << ")" << std::endl;
			
			if(_counter == 10) exit();
		}
	private:
		size_t					_counter;
	};

	class TimeoutJob1 : public basic_job_class<TimeoutJob1>
	{
	public:
		TimeoutJob1():_counter(0){}

		void timeout()
		{
			++ _counter;

			std::cout << "TimeoutJob called -- (" << _counter << ")" << std::endl;

			if(_counter == 10) exit();
		}
	private:
		size_t					_counter;
	};

	 LEMON_UNITTEST_CASE(RunQUnittest,TimeoutJobTest)
	 {
	 	runQ_service Q;

	 	for(size_t i = 0; i < 1; ++ i)
	 	{
	 		TimeoutJob::create(Q);

	 		Q.run();

	 		Q.reset();

	 		start_timer(Q,TimeoutJob1::create(Q),100);

	 		Q.run();

	 		Q.reset();
	 	}
	 }

	 class TimerCloseJob : public basic_job_class<TimerCloseJob>
	 {
	 public:
		 TimerCloseJob():_counter(0){}

		 void initialize(void*)
		 {
			 start_timer(100);
		 }

		 void timeout()
		 {
			 ++ _counter;

			 _timer.reset();

			 if(_counter == 10) stop_timer();
		 }

		 void recv(lemon_job_id /*source*/, mutable_buffer /*buff*/)
		 {
			 if(_timer.duration() / 10000000 > 4) close();
		 }

	 private:

		 lemon::timer_t									_timer;

		 size_t											_counter;
	 };

	 class TimerCloseServerJob : public basic_job_class<TimerCloseServerJob>
	 {
	 public:
		 void initialize(void*)
		 {
			 for(size_t i =0; i < 1000; ++ i)
			 {
				 _taxis.push_back(TimerCloseJob::create(service()));
			 }

			 start_timer(1000);
		 }

		 void timeout()
		 {
			 std::cout << "taxi counter :" << jobs(service()) << std::endl;

			 if(jobs(service()) == 1){ exit(); return;}

			 std::vector<job_id>::const_iterator iter,end = _taxis.end();

			 for(iter = _taxis.begin(); iter != end; ++ iter)
			 {
				 try
				 {
					 send(*iter,mutable_buffer());
				 }
				 catch(const error_info & e)
				 {
					 if(!LEMON_ERRORINOF_EQ(e.Error,LEMON_RUNQ_INVALID_JOB_ID)) throw e;
				 }
				 
			 }
		 }

	 private:
		 std::vector<job_id>										_taxis;
	 };

	 LEMON_UNITTEST_CASE(RunQUnittest,TimerCloseTest)
	 {
		 runQ_service Q;

		 for(size_t i = 0; i < 2; ++ i)
		 {
			 TimerCloseServerJob::create(Q);

			 Q.run();

			 Q.reset();
		 }
	 }

	//////////////////////////////////////////////////////////////////////////

	atomic_t globalCounter;

	class iTaxi : public basic_job_class<iTaxi>
	{
	public:

		iTaxi():counter(0){}

		void initialize(void* param)
		{
			entry_group(*(job_id*)param);

			start_timer(1000);
		}

		void multicast(lemon_job_id source, const_buffer buff)
		{
			maxCounter = buffer_cast<size_t>(buff);

			send(source, mutable_buffer());

			++ counter;

			_timer.reset();
		}

		void uninitialize()
		{
			++ globalCounter;

			LEMON_CHECK(counter == maxCounter);

			send(LEMON_SET_JOBID_REMOTE(1,1),mutable_buffer());
		}

		void timeout()
		{
			if(_timer.duration() / 10000000 > 10)
			{
				close();
			}
		}

	private:

		size_t											maxCounter;

		size_t											counter;

		lemon::timer_t									_timer;
	};



	class iTaxiGateway : public basic_job_class<iTaxiGateway>
	{
	public:

		const static int maxLoop = 10000;

		const static int maxTaxis = 1000000;

		void initialize(void*)
		{
			_loop = 0;

			_proxyCounter = 0;

			_group = create_group();

			lemon::timer_t timer;

			for(size_t i =0; i < maxTaxis; ++ i)
			{
				_taxis.push_back(iTaxi::create(service(),&_group));
			}

			time_duration duration = timer.duration();

			std::cout << "create taxis (" << _taxis.size() << ")"  

				<< " -- success(" << duration / 10000000 << "." 

				<< std::setw(6) << std::setfill('0') <<(duration % 10000000) / 10 

				<< " s)" << std::endl;

			beat();
		}

		void recv(lemon_job_id , mutable_buffer)
		{
			++ _responses;

			//free(buf);

			if(_taxis.size() == _responses)
			{
				time_duration duration = _timer.duration();

				std::cout << "recv taxi heart beat (" << _responses << ")" 

					<< " -- success(" << duration / 10000000 << "." 

					<< std::setw(6) << std::setfill('0') <<(duration % 10000000) / 10 

					<< " s)" << std::endl;

				++ _loop;
				
				if(_loop == maxLoop) { 

					start_timer(2000); return;
				}
					
				beat();

			}
		}

		void beat()
		{
			lemon::timer_t timer;

			mutable_buffer buffer = alloc(24);

			buffer_cast<size_t>(buffer) = maxLoop;

			send(_group,buffer);

			_responses = 0;

			time_duration duration = timer.duration();

			std::cout << "send taxi heart beat (" << _taxis.size() << ")"  

				<< " -- success(" << duration / 10000000 << "." 

				<< std::setw(6) << std::setfill('0') <<(duration % 10000000) / 10 

				<< " s)" << std::endl;

			_timer.reset();
		}

		void timeout()
		{
			std::cout << "taxi counter :" << jobs(service()) << std::endl;

			if(_proxyCounter == maxTaxis) exit();
		}

		void proxy(job_id, job_id target,const_buffer)
		{
			LEMON_CHECK(LEMON_JOBID_IS_REMOTE(target) != 0);

			++ _proxyCounter;
		}

	private:					

		lemon::timer_t											_timer;

		size_t													_loop;

		size_t													_responses;

		std::vector<job_id>										_taxis;

		size_t													_proxyCounter;

		job_id													_group;
	};

	LEMON_UNITTEST_CASE(RunQUnittest,iTaxiGatewayTest)
	{
		globalCounter = 0;

		runQ_service Q;

		Q.proxy(iTaxiGateway::create(Q));

		thread_group works(lemon::bind(&runQ_service::run,&Q),4);

		works.join();

		Q.reset();

		LEMON_CHECK(Q.jobs() == 0);

		LEMON_CHECK(globalCounter == iTaxiGateway::maxTaxis);
	}
}}}
