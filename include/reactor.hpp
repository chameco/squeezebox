#ifndef SQUEEZEBOX_REACTOR_HPP
#define SQUEEZEBOX_REACTOR_HPP

#include <string>
#include <memory>
#include <list>
#include <unordered_map>
#include <functional>

#include <SDL2/SDL.h>

#include "module.hpp"
#include "context.hpp"

namespace squeezebox {
	class Reactor {
		public:
			Reactor(Context &c);
			void add_module(Module &m);
			void remove_module(Module &m);
			void add_handler(int type, std::function<void(Reactor &, SDL_Event)>);
			void remove_handler(int type);
			void run();
			void stop();
		private:
			bool running;
			int last_update_time, last_draw_time, current_time;
			std::list<std::reference_wrapper<Module>> modules;
			std::unordered_map<int, std::function<void(Reactor &, SDL_Event)>> handlers;
			Context &context;
	};
}

#endif
