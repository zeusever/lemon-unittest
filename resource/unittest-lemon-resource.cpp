//#include <lemon/resource/package.hpp>
//#include <lemonxx/unittest/unittest.hpp>
//#include <lemon/resource/stringtable.hpp>
//
//
//namespace lemon{namespace resource{namespace test{
//
//	class StringTableUnittest{};
//
//	LEMON_UNITTEST_CASE(StringTableUnittest,PushGetTest)
//	{
//		StringTable table;
//
//		const char *  HelloWorld_Locale = "hello world string table";
//
//		const lemon_char_t  *HelloWorld = LEMON_TEXT("hello world string table");
//
//		const lemon_char_t  *HelloWorld1 = LEMON_TEXT("Hello world string table");
//
//		LEMON_CHECK(0 == table.Push((const lemon::byte_t*)HelloWorld_Locale,strlen(HelloWorld_Locale) + 1));
//
//		LEMON_CHECK(1 == table.Push(lemon::cbuf(HelloWorld)));
//
//		LEMON_CHECK((uint32_t)-1 == table.Push(NULL,0));
//
//		LEMON_CHECK(2 == table.Push(lemon::cbuf(HelloWorld1)));
//
//		LEMON_CHECK(3 == table.Push(lemon::cbuf(HelloWorld)));
//
//		LEMON_CHECK(4 == table.Push(lemon::cbuf(HelloWorld1)));
//
//		lemon::tuple<const lemon::byte_t*,size_t> result = table.Get(0);
//
//		
//		LEMON_CHECK(std::string((const char*)get<0>(result)) == HelloWorld_Locale);
//
//		result = table.Get(1);
//
//		const lemon_char_t* begin = (const lemon_char_t*)get<0>(result);
//
//		const lemon_char_t* end = begin + get<1>(result) / sizeof(lemon_char_t);
//
//		LEMON_CHECK(get<1>(result) % 2 == 0);
//
//		LEMON_CHECK(lemon::String(begin,end) == HelloWorld);
//
//		{
//			result = table.Get(2);
//
//			const lemon_char_t* begin = (const lemon_char_t*)get<0>(result);
//
//			const lemon_char_t* end = begin + get<1>(result) / sizeof(lemon_char_t);
//
//			LEMON_CHECK(get<1>(result) % 2 == 0);
//
//			LEMON_CHECK(lemon::String(begin,end) == HelloWorld1);
//
//			result = table.Get(2);
//		}
//
//	}
//
//	class ResourceTableUnittest{};
//
//	LEMON_UNITTEST_CASE(ResourceTableUnittest,NullStringTest)
//	{
//		ResourceTable table;
//		
//		table.NewErrorMessage(1,LEMON_TEXT(""),NULL);
//
//		const ErrorMessage * em = table.GetErrorMessage(1);
//
//		LEMON_CHECK(em->Code == 1);
//
//		LEMON_CHECK(lemon::String() == table.String(em->Name));
//
//		LEMON_CHECK(NULL == table.String(em->Description));
//	}
//
//	LEMON_UNITTEST_CASE(ResourceTableUnittest,GetSetTest)
//	{
//		ResourceTable table;
//
//		table.NewErrorMessage(1,LEMON_TEXT("hello world error message"),LEMON_TEXT("hello world"));
//
//		table.NewTraceCatalog(1,LEMON_TEXT("hello world trace catalog"),LEMON_TEXT("hello world"));
//
//		table.NewTraceMessage(1,LEMON_TEXT("hello world trace message"));
//
//		table.NewText(LEMON_TEXT("new string"),LEMON_TEXT("hello world trace message"));
//
//		lemon::String s = table.String(table.GetErrorMessage(1)->Name);
//
//		LEMON_CHECK(s == LEMON_TEXT("hello world error message"));
//
//		s = table.String(table.GetErrorMessage(1)->Description);
//
//		LEMON_CHECK(s == LEMON_TEXT("hello world"));
//
//
//		s = table.String(table.GetTraceCatalog(1)->Name);
//
//		LEMON_CHECK(s == LEMON_TEXT("hello world trace catalog"));
//
//		s = table.String(table.GetTraceCatalog(1)->Description);
//
//		LEMON_CHECK(s == LEMON_TEXT("hello world"));
//
//
//		s = table.String(table.GetTraceMessage(1)->Message);
//
//		LEMON_CHECK(s == LEMON_TEXT("hello world trace message"));
//
//		s = table.GetText(LEMON_TEXT("new string"));
//
//		LEMON_CHECK(s == LEMON_TEXT("hello world trace message"));
//	}
//
//
//
//	class PackageUnittest{};
//
//
//	LEMON_UNITTEST_CASE(PackageUnittest,LocaleGetSetTest)
//	{
//		Package package;
//
//		package.NewErrorMessage(1,LEMON_TEXT("Hello world"),NULL,LEMON_TEXT("zh_CN"));
//
//		LemonResourceInfo info = package.GetErrorMessage(1,LEMON_TEXT("zh_TW"));
//
//		LEMON_CHECK(info.Name == NULL);
//
//		LEMON_CHECK(info.Description == NULL);
//
//		info = package.GetErrorMessage(1,LEMON_TEXT("zh_CN"));
//
//		LEMON_CHECK(lemon::String(info.Name) == LEMON_TEXT("Hello world"));
//
//		LEMON_CHECK(info.Description == NULL);
//	}
//
//	class MemeoryReader : public IReader
//	{
//	public:
//		MemeoryReader(std::vector<byte_t> & buffer):_offset(0),_buffer(buffer){}
//
//		void Read(lemon::byte_t * data,size_t datalength)
//		{
//			memcpy(data,&_buffer[_offset],datalength);
//
//			_offset += datalength;
//		}
//
//	private:
//
//		size_t				_offset;
//
//		std::vector<byte_t> & _buffer;
//	};
//
//	class MemeoryWriter : public IWriter
//	{
//	public:
//		MemeoryWriter(std::vector<byte_t> & buffer):_buffer(buffer){}
//
//		void Write(const lemon::byte_t * data,size_t datalength)
//		{
//			_buffer.insert(_buffer.end(),data,data + datalength);
//		}
//
//	private:
//
//		std::vector<byte_t> & _buffer;
//	};
//
//	LEMON_UNITTEST_CASE(PackageUnittest,ReadWriterTest)
//	{
//		std::vector<byte_t> buffer;
//
//		{
//			Package package;
//
//			package.NewErrorMessage(1,LEMON_TEXT("Hello world1"),NULL,LEMON_TEXT("zh_CN"));
//
//			package.NewTraceCatalog(1,LEMON_TEXT("Hello world"),LEMON_TEXT("Hello world"),LEMON_TEXT("zh_CN"));
//
//			package.NewErrorMessage(1,LEMON_TEXT("Hello world"),NULL,LEMON_TEXT("zh_TW"));
//
//			package.NewTraceCatalog(1,LEMON_TEXT("Hello world"),LEMON_TEXT("Hello world"),LEMON_TEXT("zh_TW"));
//
//			package.NewText(LEMON_TEXT("Hello world"),LEMON_TEXT("Hello world~~~~~~~~~`"),LEMON_TEXT("zh_TW"));
//
//			MemeoryWriter writer(buffer);
//
//			package.Write(writer);
//		}
//
//		{
//			Package package;
//
//			MemeoryReader reader(buffer);
//
//			package.Read(reader);
//
//			LemonResourceInfo  info = package.GetErrorMessage(1,LEMON_TEXT("zh_CN"));
//
//			LEMON_CHECK(lemon::String(info.Name) == LEMON_TEXT("Hello world1"));
//
//			LEMON_CHECK(info.Description == NULL);
//
//			LEMON_CHECK(lemon::String(package.GetText(LEMON_TEXT("Hello world"),LEMON_TEXT("zh_TW"))) == LEMON_TEXT("Hello world~~~~~~~~~`"));
//
//			info = package.GetTraceCatalog(1,LEMON_TEXT("zh_CN"));
//
//			LEMON_CHECK(lemon::String(info.Name) == LEMON_TEXT("Hello world"));
//
//			LEMON_CHECK(lemon::String(info.Description) == LEMON_TEXT("Hello world"));
//		}
//	}
//}}}
