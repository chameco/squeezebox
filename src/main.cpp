#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

#include "reactor.hpp"
#include "module.hpp"
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

class WeaponPort {
	public:
		virtual void fire(int x, int y, int xv, int yv) =0;
		virtual void stop() {}
		virtual void update() {}
};

void game_over();

class PlayerShip : public Entity {
	public:
		PlayerShip(Context *c, int x, int y) : Entity(c, x, y, 64, 64, 100, "textures/ship.png"), velocity(0), c(c), rotation(0) {
			get_body()->SetGravityScale(0);
		}

		void destroy () {
			Entity::destroy();
			game_over();
		}

		void update() {
			get_body()->SetLinearVelocity(b2Vec2(0, velocity));
			c->set_camera_x(get_x());
			c->set_camera_y(get_y());
			weapons[0]->update();
			weapons[1]->update();
			weapons[2]->update();
			weapons[3]->update();
			collide();
		}

		void draw(Context *c, int delta) {
			b2Vec2 position = get_body()->GetPosition();
			resource.draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16, rotation * 90);
		}

		void move(bool how) {
			if (how) {
				velocity = -10;
			} else {
				velocity = 0;
			}
		}

		void rotate_clockwise() {
			weapons[0]->stop();
			weapons[1]->stop();
			weapons[2]->stop();
			weapons[3]->stop();
			rotation = (rotation + 3) % 4;
		}

		void rotate_counterclockwise() {
			weapons[0]->stop();
			weapons[1]->stop();
			weapons[2]->stop();
			weapons[3]->stop();
			rotation = (rotation + 1) % 4;
		}

		void shoot(int dir) {
			switch (dir) {
				case 0:
					weapons[(rotation + dir) % 4]->fire(get_x(), get_y() - 40, 0, -512);
					break;
				case 1:
					weapons[(rotation + dir) % 4]->fire(get_x() + 40, get_y(), 512, 0);
					break;
				case 2:
					weapons[(rotation + dir) % 4]->fire(get_x(), get_y() + 40, 0, 512);
					break;
				case 3:
					weapons[(rotation + dir) % 4]->fire(get_x() - 40, get_y(), -512, 0);
					break;
			}
		}

		void release(int dir) {
			weapons[(rotation + dir) % 4]->stop();
		}

		WeaponPort *weapons[4];
	private:
		int velocity;
		Context *c;
		int rotation;
};

Context *c;
Reactor *r;
PlayerShip *PLAYER;
EntityManager *ENTITY_MANAGER;
World *WORLD;
GuiManager *GUI_MANAGER;
GuiElement *TITLE;
GuiElement *GAME_OVER;
GuiElement *BUTTON_ENTER;
Sound *GREEN_FIRE;
Sound *YELLOW_FIRE;
Sound *RED_FIRE;
Sound *BLUE_FIRE;

class ProjectileGreen : public Projectile {
	public:
		ProjectileGreen(Context *c, int x, int y, int xv, int yv) : Projectile(c, x, y, 8, 8, xv, yv, 8, "textures/bullet_green.png") {
			GREEN_FIRE->play();
		}
		class Port : public WeaponPort {
			public:
				void fire(int x, int y, int xv, int yv) {
					ENTITY_MANAGER->add_entity(new ProjectileGreen(c, x, y, xv, yv));
				}
		};
};

class ProjectileYellow : public Projectile {
	public:
		ProjectileYellow(Context *c, int x, int y, int xv, int yv) : Projectile(c, x, y, 8, 8, xv, yv, 1, "textures/bullet_yellow.png") {
			YELLOW_FIRE->play();
		}
		class Port : public WeaponPort {
			public:
				Port() : firing(false), timer(0) {}
				void update() {
					if (firing) {
						timer = (timer + 1) % 5;
						if (timer == 0) {
							ENTITY_MANAGER->add_entity(new ProjectileYellow(c, x, y, xv, yv));
						}
					}
				}
				void fire(int px, int py, int pxv, int pyv) {
					firing = true;
					x = px;
					y = py;
					xv = pxv;
					yv = pyv;
				}
				void stop() {
					firing = false;
				}
			private:
				bool firing;
				int x, y, xv, yv;
				int timer;
		};
};

class ProjectileRed : public Projectile {
	public:
		ProjectileRed(Context *c, int x, int y, int xv, int yv) : Projectile(c, x, y, 8, 8, xv, yv, 8, "textures/bullet_red.png") {
			RED_FIRE->play();
		}
		class Port : public WeaponPort {
			public:
				void fire(int x, int y, int xv, int yv) {
					ENTITY_MANAGER->add_entity(new ProjectileRed(c, x, y, xv, yv));
				}
		};
};

class ProjectileBlue : public Projectile {
	public:
		ProjectileBlue(Context *c, int x, int y, int w, int h, int xv, int yv) : Projectile(c, x, y, w, h, xv, yv, 6, "textures/bullet_blue.png") {
			BLUE_FIRE->play();
		}
		void draw(Context *c, int delta) {
			b2Vec2 position = get_body()->GetPosition();
			if (get_xv() < 0) {
				resource.draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16, 90);
			} else if (get_xv() > 0) {
				resource.draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16, 270);
			}
			if (get_yv() < 0) {
				resource.draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16, 0);
			} else if (get_yv() > 0) {
				resource.draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16, 180);
			}
		}
		void destroy() {
			Entity::destroy();
			ENTITY_MANAGER->add_entity(new ProjectileRed(c, get_x() - 24, get_y(), -512, 0));
			ENTITY_MANAGER->add_entity(new ProjectileRed(c, get_x() + 24, get_y(), 512, 0));
			ENTITY_MANAGER->add_entity(new ProjectileRed(c, get_x(), get_y() - 24, 0, -512));
			ENTITY_MANAGER->add_entity(new ProjectileRed(c, get_x(), get_y() + 24, 0, 512));
		}
		class Port : public WeaponPort {
			public:
				void fire(int x, int y, int xv, int yv) {
					if (xv < 0) {
						ENTITY_MANAGER->add_entity(new ProjectileBlue(c, x - 24, y, 32, 32, 0, 256));
					} else if (xv > 0) {
						ENTITY_MANAGER->add_entity(new ProjectileBlue(c, x + 24, y, 32, 32, 0, -256));
					}
					if (yv < 0) {
						ENTITY_MANAGER->add_entity(new ProjectileBlue(c, x, y - 24, 32, 32, -256, 0));
					} else if (yv > 0) {
						ENTITY_MANAGER->add_entity(new ProjectileBlue(c, x, y + 24, 32, 32, 256, 0));
					}
				}
		};
};

void main_key_handler(Reactor &r, SDL_Event e) {
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
		case SDLK_a:
			if (e.type == SDL_KEYDOWN) {
				PLAYER->rotate_counterclockwise();
			}
			break;
		case SDLK_d:
			if (e.type == SDL_KEYDOWN) {
				PLAYER->rotate_clockwise();
			}
			break;
		case SDLK_UP:
			if (e.type == SDL_KEYDOWN) {
				PLAYER->shoot(0);
			} else {
				PLAYER->release(0);
			}
			break;
		case SDLK_RIGHT:
			if (e.type == SDL_KEYDOWN) {
				PLAYER->shoot(1);
			} else {
				PLAYER->release(1);
			}
			break;
		case SDLK_DOWN:
			if (e.type == SDL_KEYDOWN) {
				PLAYER->shoot(2);
			} else {
				PLAYER->release(2);
			}
			break;
		case SDLK_LEFT:
			if (e.type == SDL_KEYDOWN) {
				PLAYER->shoot(3);
			} else {
				PLAYER->release(3);
			}
			break;
	}
}

void start_main_game() {
	PLAYER = new PlayerShip(c, 0, -160);
	PLAYER->weapons[0] = new ProjectileGreen::Port();
	PLAYER->weapons[1] = new ProjectileYellow::Port();
	PLAYER->weapons[2] = new ProjectileRed::Port();
	PLAYER->weapons[3] = new ProjectileBlue::Port();

	ENTITY_MANAGER = new EntityManager(c, "levels/world");
	ENTITY_MANAGER->add_entity(PLAYER);

	WORLD = new World(c, "levels/world");

	//MusicManager music("music/test.ogg");

	r->add_module(WORLD);
	r->add_module(ENTITY_MANAGER);
	r->add_handler(SDL_KEYDOWN, main_key_handler);
	r->add_handler(SDL_KEYUP, main_key_handler);
}

void game_over() {
	r->remove_module(WORLD);
	r->remove_module(ENTITY_MANAGER);
	r->remove_handler(SDL_KEYDOWN);
	r->remove_handler(SDL_KEYUP);

	GAME_OVER = new GuiElement(c, c->get_screen_width()/2 - (9 * 32), c->get_screen_height()/2 - 32, 8, 8, new TextResource("Game Over", 8));
	GUI_MANAGER->add_gui_element(GAME_OVER);
}

class GuiElementButton : public GuiElement {
	public:
		GuiElementButton(Context *c, int x, int y, int w, int h, string path) : GuiElement(c, x, y, w, h, new ImageResource(path)) {}
		void on_left_click() {
			TITLE->destroy();
			destroy();
			start_main_game();
		}
};

int main(int argc, char *argv[]) {
	c = new Context("test", 30.0);
	r = new Reactor(c);

	GREEN_FIRE = new Sound("sounds/green_fire.wav");
	YELLOW_FIRE = new Sound("sounds/green_fire.wav");
	RED_FIRE = new Sound("sounds/green_fire.wav");
	BLUE_FIRE = new Sound("sounds/green_fire.wav");

	GUI_MANAGER = new GuiManager();

	TITLE = new GuiElement(c, c->get_screen_width()/2 - (13 * 32), c->get_screen_height()/2 - 32, 8, 8, new TextResource("One Direction", 8));
	GUI_MANAGER->add_gui_element(TITLE);

	BUTTON_ENTER = new GuiElementButton(c, 100, 100, 64, 64, "textures/ship.png");
	GUI_MANAGER->add_gui_element(BUTTON_ENTER);

	r->add_module(GUI_MANAGER);
	r->add_handler(SDL_MOUSEBUTTONDOWN, [&] (Reactor &r, SDL_Event e) {GUI_MANAGER->handler(r, e);});
	r->run();
	return 0;
}
