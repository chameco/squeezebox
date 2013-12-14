#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

#include "reactor.hpp"
#include "entity.hpp"
#include "projectile.hpp"
#include "entity_manager.hpp"
#include "gui_element.hpp"
#include "gui_manager.hpp"
#include "text_resource.hpp"
#include "music_manager.hpp"
#include "world.hpp"

using namespace squeezebox;
using namespace std;

class PlayerShip : public Entity {
	public:
		PlayerShip(Context *c, int x, int y) : Entity(c, x, y, 64, 64, "textures/ship.png"), velocity(0), c(c) {
			get_body()->SetGravityScale(0);
		}
		void update() {
			get_body()->SetLinearVelocity(b2Vec2(0, velocity));
			c->set_camera_x(get_x());
			c->set_camera_y(get_y());
			collide();
		}
		void move(bool how) {
			if (how) {
				velocity = -10;
			} else {
				velocity = 0;
			}
		}
	private:
		int velocity;
		Context *c;
};

PlayerShip *PLAYER;
EntityManager *ENTITY_MANAGER;
Context c("test", 30.0);

void key_handler(Reactor &r, SDL_Event e) {
	if (e.key.repeat) {
		return;
	}
	switch (e.key.keysym.sym) {
		case SDLK_SPACE:
			if (e.type == SDL_KEYDOWN) {
				PLAYER->move(true);
			} else {
				PLAYER->move(false);
			}
			break;
		case SDLK_UP:
			if (e.type == SDL_KEYDOWN) {
				ENTITY_MANAGER->add_entity(new Projectile(&c, PLAYER->get_x(), PLAYER->get_y() - 40, 8, 8, 0, -512, "textures/bullet_green.png"));
			}
			break;
		case SDLK_LEFT:
			if (e.type == SDL_KEYDOWN) {
				ENTITY_MANAGER->add_entity(new Projectile(&c, PLAYER->get_x() - 40, PLAYER->get_y(), 8, 8, -512, 0, "textures/bullet_blue.png"));
			}
			break;
		case SDLK_RIGHT:
			if (e.type == SDL_KEYDOWN) {
				ENTITY_MANAGER->add_entity(new Projectile(&c, PLAYER->get_x() + 40, PLAYER->get_y(), 8, 8, 512, 0, "textures/bullet_yellow.png"));
			}
			break;
		case SDLK_DOWN:
			if (e.type == SDL_KEYDOWN) {
				ENTITY_MANAGER->add_entity(new Projectile(&c, PLAYER->get_x(), PLAYER->get_y() + 40, 8, 8, 0, 512, "textures/bullet_red.png"));
			}
			break;
	}
}

int main(int argc, char *argv[]) {
	Reactor r(&c);

	PLAYER = new PlayerShip(&c, 0, -160);
	//Entity slime1(c, 0, 0, 32, 32, "textures/slime.png");
	//Entity slime2(c, 100, -300, 32, 32, "textures/slime.png");
	//Entity slime3(c, -100, -300, 32, 32, "textures/slime.png");
	
	World w(&c, "levels/world");

	ENTITY_MANAGER = new EntityManager(&c, "levels/world");
	ENTITY_MANAGER->add_entity(PLAYER);
	//m.add_entity(&slime1);
	//m.add_entity(&slime2);
	//m.add_entity(&slime3);

	TextResource t("Hello World", 8);
	GuiElement hello(&c, 0, 0, 8, 8, &t);

	GuiManager g;
	g.add_gui_element(&hello);

	//MusicManager music("music/test.ogg");

	r.add_module(&w);
	r.add_module(ENTITY_MANAGER);
	r.add_module(&g);
	r.add_handler(SDL_KEYDOWN, key_handler);
	r.add_handler(SDL_KEYUP, key_handler);
	r.run();
	return 0;
}
