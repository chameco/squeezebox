#include "reactor.hpp"

#include <string>
#include <vector>
#include <map>
#include <functional>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "module.hpp"
#include "context.hpp"

using namespace squeezebox;
using namespace std;

Reactor::Reactor(string name) : running(false), context(name) {
	add_handler(SDL_QUIT, [](Reactor &r, SDL_Event e) { r.stop(); });
}

void Reactor::add_module(Module *m) {
	modules.push_back(m);
}

void Reactor::add_handler(int type, function<void(Reactor &, SDL_Event)> cb) {
	handlers[type] = cb;
}

void Reactor::remove_handler(int type) {
	handlers.erase(handlers.find(type));
}

void Reactor::run() {
	running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (handlers.find(event.type) != handlers.end()) {
				handlers[event.type](*this, event);
			}
		}

		current_time = SDL_GetTicks();
		if (current_time - last_time > 40) {
			for (Module *m : modules) {
				m->update(context);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		for (Module *m : modules) {
			m->draw(context, current_time - last_time);
		}

		context.update_screen();
	}
}

void Reactor::stop() {
	running = false;
}
