#include "reactor.hpp"

#include <string>
#include <memory>
#include <iostream>
#include <list>
#include <unordered_map>
#include <functional>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "module.hpp"
#include "context.hpp"

using namespace squeezebox;
using namespace std;

Reactor::Reactor(Context &c) : running(false), context(c) {
	add_handler(SDL_QUIT, [](Reactor &r, SDL_Event e) { r.stop(); });
	last_update_time = last_draw_time = current_time = 0;
}

void Reactor::add_module(Module &m) {
	modules.push_back(m);
}

void Reactor::remove_module(Module &m) {
	m.destroy();
}

void Reactor::add_handler(int type, function<void(Reactor &, SDL_Event)> cb) {
	handlers[type] = cb;
}

void Reactor::remove_handler(int type) {
	if (handlers.find(type) != handlers.end()) {
		handlers.erase(handlers.find(type));
	}
}

static bool destroy_module_predicate(Module &m) {
	return !m.is_alive();
}

void Reactor::run() {
	running = true;
	int delta = 10;
	int accumulator = 0;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (handlers.find(event.type) != handlers.end()) {
				handlers[event.type](*this, event);
			}
		}

		int new_time = SDL_GetTicks();
		int frame_time = new_time - current_time;
		current_time = new_time;

		accumulator += frame_time;

		while (accumulator >= delta) {
			modules.remove_if(destroy_module_predicate);
			for (Module &m : modules) {
				m.update(context);
			}
			context.update_physics(delta/1000.0f);
			accumulator -= delta;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		for (Module &m : modules) {
			m.draw(context, current_time - last_draw_time);
		}

		context.update_screen();
	}
}

void Reactor::stop() {
	running = false;
}
