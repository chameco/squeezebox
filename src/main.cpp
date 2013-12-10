#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

#include "reactor.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "gui_element.hpp"
#include "gui_manager.hpp"
#include "text_resource.hpp"
#include "world.hpp"

using namespace squeezebox;
using namespace std;

Entity *PLAYER;

void key_handler(Reactor &r, SDL_Event e) {
	if (e.key.repeat) {
		return;
	}
	switch (e.key.keysym.sym) {
		case SDLK_a:
			if (e.type == SDL_KEYDOWN) {
				PLAYER->set_x_velocity(-256);
			} else {
				PLAYER->set_x_velocity(0);
			}
			break;
		case SDLK_d:
			if (e.type == SDL_KEYDOWN) {
				PLAYER->set_x_velocity(256);
			} else {
				PLAYER->set_x_velocity(0);
			}
			break;
		case SDLK_SPACE:
			if (e.type == SDL_KEYDOWN) {
				PLAYER->impulse_y(-2048);
			}
	}
}

int main(int argc, char *argv[]) {
	Context c("test", 10.0);
	Reactor r(c);

	PLAYER = new Entity(c, 0, -160, 32, 32, "wizard.png");
	Entity slime1(c, 0, 0, 32, 32, "slime.png");
	Entity slime2(c, 100, -300, 32, 32, "slime.png");
	Entity slime3(c, -100, -300, 32, 32, "slime.png");
	
	World w(c, "world");

	EntityManager m;
	m.add_entity(PLAYER);
	m.add_entity(&slime1);
	m.add_entity(&slime2);
	m.add_entity(&slime3);

	TextResource t("Hello World", 8);
	GuiElement hello(c, 0, 0, 8, 8, &t);

	GuiManager g;
	g.add_gui_element(&hello);

	r.add_module(&w);
	r.add_module(&m);
	r.add_module(&g);
	r.add_handler(SDL_KEYDOWN, key_handler);
	r.add_handler(SDL_KEYUP, key_handler);
	r.run();
	return 0;
}
