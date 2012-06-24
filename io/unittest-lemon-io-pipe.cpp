#include <string>
#include <lemonxx/io/pipe.hpp>
#include <lemonxx/utility/utility.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{
	struct PipeUnittest{};

	const static char * pipeName = "\\\\.\\pipe\\{F0D6858B-8888-4D0A-8F6B-997C3B1B4E36}";

	void SyncPipeClient()
	{
		pipe::client c(pipeName);

		const static char * message = "hello the pipe world!!!!!!!";

		c.write(cbuf(message));
	}

	LEMON_UNITTEST_CASE(PipeUnittest,SyncPipTest)
	{
		pipe::server server(pipeName);

		lemon::thread_t t(&SyncPipeClient);

		lemon::thread_t t2(&SyncPipeClient);

		server.accept();

		byte_t buffer[256];

		size_t length = server.read(buf(buffer));

		std::cout << std::string(buffer,buffer + length) << std::endl;

		server.accept();

		length = server.read(buf(buffer));

		std::cout << std::string(buffer,buffer + length) << std::endl;

		t.join();

		t2.join();
	}
}}}