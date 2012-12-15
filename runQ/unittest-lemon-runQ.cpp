#include <lemonxx/function/bind.hpp>
#include <lemon/runQ/assembly.h>
#include <lemonxx/runQ/runQ.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace runQ{namespace test{

	class RunQUnittest{};

	class ExitJob : public basic_job_class<ExitJob>
	{
	public:
		void initialize()
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

	 		runQ::send(Q,LEMON_INVALID_JOB_ID,id,mutable_buffer());

	 		Q.run();
	
	 		Q.reset();

	 		LEMON_CHECK(Q.jobs() == 0);
	 	}
	 }

	class TimeoutJob : public basic_job_class<TimeoutJob>
	{
	public:
		TimeoutJob():_counter(0){}

		void initialize()
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

		 void initialize()
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
		 void initialize()
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

		void initialize()
		{
			start_timer(2000);
		}

		void recv(lemon_job_id source, mutable_buffer buff)
		{
			maxCounter = buffer_cast<size_t>(buff);

			buffer_cast<size_t>(buff) = counter ++;

			send(source,buff);

			_timer.reset();
		}

		void uninitialize()
		{
			++ globalCounter;

			LEMON_CHECK(counter == maxCounter);
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

		const static int maxLoop = 10;

		const static int maxTaxis = 300000;

		void initialize()
		{
			_loop = 0;

			for(size_t i =0; i < maxTaxis; ++ i)
			{
				_taxis.push_back(iTaxi::create(service()));
			}

			beat();
		}

		void recv(lemon_job_id id, mutable_buffer buff)
		{
			++ _responses;

			id;

			LEMON_CHECK(buffer_cast<size_t>(buff) == _loop);

			free(buff);

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
			_timer.reset();

			lemon::timer_t timer;

			std::vector<job_id>::const_iterator iter,end = _taxis.end();

			for(iter = _taxis.begin(); iter != end; ++ iter)
			{
				mutable_buffer buffer = alloc(24);

				buffer_cast<size_t>(buffer) = maxLoop;

				send(*iter,buffer);
			}

			_responses = 0;

			time_duration duration = timer.duration();

			std::cout << "send taxi heart beat (" << _taxis.size() << ")"  

				<< " -- success(" << duration / 10000000 << "." 

				<< std::setw(6) << std::setfill('0') <<(duration % 10000000) / 10 

				<< " s)" << std::endl;
		}

		void timeout()
		{
			std::cout << "taxi counter :" << jobs(service()) << std::endl;

			if(jobs(service()) == 1) exit();
		}

	private:					

		lemon::timer_t											_timer;

		size_t													_loop;

		size_t													_responses;

		std::vector<job_id>										_taxis;
	};

	LEMON_UNITTEST_CASE(RunQUnittest,iTaxiGatewayTest)
	{
		globalCounter = 0;

		runQ_service Q;

		iTaxiGateway::create(Q);

		thread_group works(lemon::bind(&runQ_service::run,&Q),4);

		works.join();

		Q.reset();

		LEMON_CHECK(Q.jobs() == 0);

		LEMON_CHECK(globalCounter == iTaxiGateway::maxTaxis);
	}

}}}
