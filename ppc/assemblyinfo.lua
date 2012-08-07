assembly = 
   {
   -- the project name
   name = "unittest-lemon-ppc";


   -- the project guid
   guid = "{ 0x612c9e12, 0x2142, 0x4a7d, { 0x8a, 0x11, 0x3c, 0x2e, 0x8e, 0x58, 0xc8, 0x53 } }";

   tracelog = 
	{
		macro = "UNITTEST_LEMON_PPC_TRACE";

		catalog = 
			{
				{
					name = "TEST_A";
				};

				{
					name = "TEST_B";
				};
			};
	};

   -- the project errorcode define
   
   errorcode=
      {
		{
			name = "OPEN_LUA_ERROR";
		};

		{
			name = "LUA_PCALL_ERROR";
		};

		{
			name = "LUA_DOFILE_ERROR";
		};

		{
			name = "LUA_DUMPLICATE_CLASS_FUNCTION_DEF";
		};
	  };
   }