#include <string>
#include <lemonxx/io/pipe.hpp>
#include <lemonxx/utility/utility.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{
	struct PipeUnittest{};

	const static char * pipeName = "\\\\.\\pipe\\{F0D6858B-8888-4D0A-8F6B-997C3B1B4E36}";

	void AsyncRead(size_t	/*numberOfBytesTransferred*/,const LemonErrorInfo &/*errorCode*/)
	{

	}

	void AsyncWrite(size_t	/*numberOfBytesTransferred*/,const LemonErrorInfo &/*errorCode*/)
	{

	}

	void SyncPipeClient()
	{
		io_device device;

		pipe::client c(pipeName,device);

		const static char * message = "hello the pipe world!!!!!!!";

		static char buffer[256] = {0};

		c.async_read(lemon::buf(buffer),&AsyncRead);

		c.write(lemon::cbuf(message));

		device.run();
	}

	LEMON_UNITTEST_CASE(PipeUnittest,SyncPipTest)
	{
		pipe::server server(pipeName);

		lemon::thread_t t(&SyncPipeClient);

		server.accept();

		byte_t buffer[256];

		size_t length = server.read(buf(buffer));

		std::cout << std::string(buffer,buffer + length) << std::endl;

		server.write(cbuf(buffer));

		t.join();
	}
}}}