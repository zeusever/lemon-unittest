assembly = 
   {
   -- the project name
   name = "unittest-lemon-diagnosis";

   description = "the lemon sys library";

   company = "yayanyang@gmail.com";

   -- the project guid
   guid = "{ 0x612c9e2e, 0x2341, 0x4a7d, { 0x8a, 0x11, 0x3c, 0x2e, 0x8e, 0x58, 0xc8, 0x53 } }";

   dtrace = 
	  {
		name = "LEMON_SYS_TRACE";
		
		catalog = 
		{
			{
				name = "FILESYSTEM";

				description = "filesytem trace";
			};

			{
				name = "COROUTINE";

				description = "coruinte trace";
			};
		}; 
	  };

   -- the project errorcode define
   errorcode=
      {
		{
			name = "NOT_IMPLEMENT";
		};
		
		{
			name = "UUID_CREATE_ERROR";
		};
		
		{
			name = "BUFFER_TOO_SMALL";
		};
		
		{
			name = "UNITTEST_FAILED";
		};
		
		{
			name = "RESOURCE_ERROR";
		};
		
		{
			name = "INVALID_UUID_STRING";
		};
		
		{
			name = "LD_ERROR";
		};
		
		
		{
			name = "LD_GET_METHOD_ERROR";
		};
		
		{
			name = "COROUTINE_EXIT";
		};
		
		{
			name = "OPEN_LUA_ERROR";
		};
		
		{
			name = "PO_TYPE_ERROR";
		};
		
		{
			name = "LUA_DOFILE_ERROR";
		};
		
		{
			name = "THREAD_GROUP_MULTI_ADD";
		};

		{
			name = "NOT_FOUND_ENV";
		};
		
		{
			name = "INVALID_VERSION_STRING";
		};
      };
   }