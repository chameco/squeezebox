#ifndef SQUEEZEBOX_REACTOR_HPP
#define SQUEEZEBOX_REACTOR_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include <SDL2/SDL.h>

#include "module.hpp"
#include "context.hpp"

using namespace std;

namespace squeezebox {
	class Reactor {
		public:
			Reactor(const Context &c);
			~Reactor();
			Context &get_context() { return context; }
			void add_module(Module *m);
			void add_handler(int type, function<void(Reactor &, SDL_Event)>);
			void remove_handler(int type);
			void run();
			void stop();
		private:
			bool running;
			int last_update_time, last_draw_time, current_time;
			vector<Module *> modules;
			unordered_map<int, function<void(Reactor &, SDL_Event)>> handlers;
			Context context;
	};
}

#endif
