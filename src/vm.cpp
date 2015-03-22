#include "vm.hpp"

#include <string>
#include <iostream>
#include <cstring>

using namespace squeezebox;
using namespace std;

VM::VM() {
	lvm = luaL_newstate();
	luaL_openlibs(lvm);
}

void VM::close() {
	lua_close(lvm);
}

void VM::load_file(string path) {
	int error = luaL_loadfile(lvm, path.c_str()) || lua_pcall(lvm, 0, 0, 0);
	if (error != 0) {
		cerr << "Error: Lua script at " << path << " failed with error " << lua_tostring(lvm, -1) << endl;
		lua_pop(lvm, 1);
	}
}

void VM::eval_expression(string expr) {
	int error = luaL_loadbuffer(lvm, expr.c_str(), expr.length(), "squeezebox") || lua_pcall(lvm, 0, 0, 0);
	if (error != 0) {
		cerr << "Error: Lua expression " << expr << " failed with error " << lua_tostring(lvm, -1) << endl;
		lua_pop(lvm, 1);
	}
}

void VM::call_function(string name) {
	lua_getglobal(lvm, name.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		cerr << "Error: Lua variable " << name << " does not exist" << endl;
	} else {
		lua_pcall(lvm, 0, 0, 0);
	}
}

int VM::get_table_size(string name) {
	lua_getglobal(lvm, name.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		cerr << "Error: Lua variable " << name << " does not exist" << endl;
		return -1;
	} else {
		lua_len(lvm, -1);
		return lua_tonumber(lvm, -1);
	}
}

void VM::descend_table(string name) {
	if (!lua_istable(lvm, -2)) {
		cerr << "Error: element \"" << name << "\" is not a table" << endl;
		return;
	} else {
		lua_pushstring(lvm, name.c_str());
		lua_gettable(lvm, -2);
	}
}

void VM::descend_array(int index) {
	if (!lua_istable(lvm, -1)) {
		cerr << "Error: element " << index << " is not an array" << endl;
		return;
	} else {
		lua_rawgeti(lvm, -1, index);
	}
}

double VM::get_numeric(string name) {
	lua_getglobal(lvm, name.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		return 0;
	} else {
		return lua_tonumber(lvm, -1);
	}
}

double VM::get_numeric(string name, string table) {
	lua_getglobal(lvm, table.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		return 0;
	} else {
		return get_field_numeric(name);
	}
}

double VM::get_numeric(string name, string table, function<void()> thunk) {
	lua_getglobal(lvm, table.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		return 0;
	} else {
		thunk();
		return get_field_numeric(name);
	}
}

double VM::get_numeric(int index, string table) {
	lua_getglobal(lvm, table.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		return 0;
	} else {
		return get_field_numeric(index);
	}
}

double VM::get_numeric(int index, string table, function<void()> thunk) {
	lua_getglobal(lvm, table.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		return 0;
	} else {
		thunk();
		return get_field_numeric(index);
	}
}

string VM::get_string(string name) {
	lua_getglobal(lvm, name.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		return "";
	} else {
		return lua_tostring(lvm, -1);
	}
}

string VM::get_string(string name, string table) {
	lua_getglobal(lvm, table.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		return "";
	} else {
		return get_field_string(name);
	}
}

string VM::get_string(string name, string table, function<void()> thunk) {
	lua_getglobal(lvm, table.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		return "";
	} else {
		thunk();
		return get_field_string(name);
	}
}

string VM::get_string(int index, string table) {
	lua_getglobal(lvm, table.c_str());
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		return "";
	} else {
		return get_field_string(index);
	}
}

string VM::get_string(int index, string table, function<void()> thunk) {
	lua_getglobal(lvm, table.c_str());
	thunk();
	if (lua_type(lvm, lua_gettop(lvm)) == LUA_TNIL) {
		cerr << "Error: Lua variable " << table << " does not exist" << endl;
		return "";
	} else {
		return get_field_string(index);
	}
}
