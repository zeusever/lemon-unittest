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