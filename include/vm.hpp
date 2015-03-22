#ifndef SQUEEZEBOX_VM_HPP
#define SQUEEZEBOX_VM_HPP

#include <string>
#include <iostream>
#include <functional>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace squeezebox {
	class VM {
		public:
			VM();
			void close();
			void load_file(std::string path);
			void eval_expression(std::string expr);
			void call_function(std::string name);
			int get_table_size(std::string name);
			void descend_table(std::string name);
			void descend_array(int index);
			double get_numeric(std::string name);
			double get_numeric(std::string name, std::string table);
			double get_numeric(std::string name, std::string table, std::function<void()> thunk);
			double get_numeric(int index, std::string table);
			double get_numeric(int index, std::string table, std::function<void()> thunk);
			std::string get_string(std::string name);
			std::string get_string(std::string name, std::string table);
			std::string get_string(std::string name, std::string table, std::function<void()> thunk);
			std::string get_string(int index, std::string table);
			std::string get_string(int index, std::string table, std::function<void()> thunk);
		private:
			double get_field_numeric(std::string key) {
				double result;
				lua_pushstring(lvm, key.c_str());
				lua_gettable(lvm, -2);
				if (!lua_isnumber(lvm, -1)) {
					return 0;
				}
				result = lua_tonumber(lvm, -1);
				lua_pop(lvm, 1);
				return result;
			}

			double get_field_numeric(int index) {
				double result;
				lua_rawgeti(lvm, -1, index);
				if (!lua_isnumber(lvm, -1)) {
					return 0;
				}
				result = lua_tonumber(lvm, -1);
				lua_pop(lvm, 1);
				return result;
			}

			std::string get_field_string(std::string key) {
				std::string result;
				lua_pushstring(lvm, key.c_str());
				lua_gettable(lvm, -2);
				if (!lua_isstring(lvm, -1)) {
					return "";
				}
				result = lua_tostring(lvm, -1);
				lua_pop(lvm, 1);
				return result;
			}

			std::string get_field_string(int index) {
				std::string result;
				lua_rawgeti(lvm, -1, index);
				if (!lua_isstring(lvm, -1)) {
					return "";
				}
				result = lua_tostring(lvm, -1);
				lua_pop(lvm, 1);
				return result;
			}
			lua_State *lvm;
	};
}

#endif
