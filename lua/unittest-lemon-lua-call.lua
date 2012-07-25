function hello()
	return "hello the lemon::luabind world"
end

function hello_false()
	return "hello the lemon::luabind world",false
end

function hello_true()
	return "hello the lemon::luabind world",true
end

function say_hello(msg,status)
	return msg,status
end

function echo_number(n) return n end

function test(a)
	print(a)
	
	return test
end

function c_extension_hello()
	
	extensions_test.hello()
	
	extensions_test.hello2()
	
	extensions_test.hello3(type(c_extension_hello))
	
	print(extensions_test.hello3("hello world!!!!!!!!!!!"))
	
end


function cpp_test(t)
	--for n = 1, counter do
		t.hello()
		
		t.hello2()
	--end
end

function cpp_test1()
	extensions_test.hello()
end


