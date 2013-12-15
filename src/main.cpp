#include <iostream>
#include <cstdlib>
#include <ctime>

#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

#include "reactor.hpp"
#include "module.hpp"
#include "entity.hpp"
#include "projectile.hpp"
#include "entity_manager.hpp"
#include "gui_element.hpp"
#include "gui_manager.hpp"
#include "image_resource.hpp"
#include "text_resource.hpp"
#include "color_resource.hpp"
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

EntityManager *ENTITY_MANAGER;
GuiManager *GUI_MANAGER;
GuiElement *TITLE;
GuiElement *YOGO;
GuiElement *GAME_OVER;
GuiElement *WAVES_CLEARED;
GuiElement *BUTTON_ENTER;
GuiElement *CHARGES;
GuiElement *TELEPORTS[3];
int TELEPORT_INDEX = 2;
GuiElement *TIME_TEXT;
GuiElement *TIME_NUMBER;

Sound *GREEN_FIRE;
Sound *YELLOW_FIRE;
Sound *RED_FIRE;
Sound *BLUE_FIRE;
Sound *BLIP;
Sound *BLOOP;
Sound *SUCCESS[2];

int NUM_SHIPS = 0;
bool FLAG_SWITCH = false;
int TIME = 20000;
int WAVE = 0;

void game_over();
void switch_wave();

class PlayerShip : public Entity {
	public:
		PlayerShip(Context *c, int x, int y) : Entity(c, x, y, 64, 64, 1, new ImageResource("textures/ship.png")), velocity(0), c(c), rotation(0), engine("textures/engine.png") {
			get_body()->SetGravityScale(0);
		}

		void take_damage (int d) {}

		void update() {
			get_body()->SetLinearVelocity(b2Vec2(0, velocity));
			c->set_camera_x(get_x());
			c->set_camera_y(get_y());
			weapons[0]->update();
			weapons[1]->update();
			weapons[2]->update();
			weapons[3]->update();
			collide();
			if (NUM_SHIPS == 0) {
				FLAG_SWITCH = true;
			}
		}

		void draw(Context *c, int delta) {
			b2Vec2 position = get_body()->GetPosition();
			resource->draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16, rotation * 90);
			engine.draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16 + 32);
		}

		void move(bool how) {
			if (how) {
				velocity = -10;
			} else {
				velocity = 0;
			}
		}

		int get_velocity() { return velocity; } 
		void rotate_clockwise() {
			weapons[0]->stop();
			weapons[1]->stop();
			weapons[2]->stop();
			weapons[3]->stop();
			rotation = (rotation + 3) % 4;
			BLIP->play();
		}

		void rotate_counterclockwise() {
			weapons[0]->stop();
			weapons[1]->stop();
			weapons[2]->stop();
			weapons[3]->stop();
			rotation = (rotation + 1) % 4;
			BLIP->play();
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
		ImageResource engine;
};

class Switcher : public Module {
	public:
		void update(Context *c) {
			TIME -= 1;
			string time = to_string(TIME);
			((TextResource *) TIME_NUMBER->get_resource())->set_text(time);
			if (TIME <= 0) {
				game_over();
			}
			if (FLAG_SWITCH) {
				switch_wave();
				FLAG_SWITCH = 0;
			}
		}
		void draw(Context *c, int delta) {}
};

Switcher *SWITCHER;

class EnemyShip : public Entity {
	public:
		EnemyShip(Context *c, int x, int y, string t) : Entity(32, 32, t == "blue" ? 100 : 16, new ImageResource("textures/enemy_" + t + ".png")), type(t) {
			body_def.type = b2_kinematicBody;
			body_def.position.Set(x/16.0f, y/16.0f);
			body_def.fixedRotation = true;
			body = c->get_world()->CreateBody(&body_def);
			body->SetUserData((void *) this);
			box.SetAsBox(w, h);
			fixture_def.shape = &box;
			fixture_def.density = 1.4f;
			fixture_def.friction = 0.3f;
			body->CreateFixture(&fixture_def);
			NUM_SHIPS += 1;
		}
		void destroy() {
			Entity::destroy();
			NUM_SHIPS -= 1;
		}
		string type;
};

Context *c;
Reactor *r;
PlayerShip *PLAYER;

class ProjectileGreen : public Projectile {
	public:
		ProjectileGreen(Context *c, int x, int y, int xv, int yv) : Projectile(c, x, y, 8, 8, xv, yv, 100, new ImageResource("textures/bullet_green.png")) {
			GREEN_FIRE->play();
		}
		void collide() {
			for (Entity *e : contacts) {
				if (e != NULL) {
					if (EnemyShip *es = dynamic_cast<EnemyShip *>(e)) {
						if (es->type == "red") {
							TIME -= 100;
						}
						if (es->type != "green") {
							lifespan = 0;
							continue;
						}
					}
					if (e->is_alive()) {
						e->take_damage(strength);
					}
				}
				lifespan = 0;
			}
		}
		class Port : public WeaponPort {
			public:
				void fire(int x, int y, int xv, int yv) {
					if (xv < 0) {
						ENTITY_MANAGER->add_entity(new ProjectileGreen(c, x, y + 16, xv, yv));
						ENTITY_MANAGER->add_entity(new ProjectileGreen(c, x, y - 16, xv, yv));
					} else if (xv > 0) {
						ENTITY_MANAGER->add_entity(new ProjectileGreen(c, x, y + 16, xv, yv));
						ENTITY_MANAGER->add_entity(new ProjectileGreen(c, x, y - 16, xv, yv));
					}
					if (yv < 0) {
						ENTITY_MANAGER->add_entity(new ProjectileGreen(c, x + 16, y, xv, yv));
						ENTITY_MANAGER->add_entity(new ProjectileGreen(c, x - 16, y, xv, yv));
					} else if (yv > 0) {
						ENTITY_MANAGER->add_entity(new ProjectileGreen(c, x + 16, y, xv, yv));
						ENTITY_MANAGER->add_entity(new ProjectileGreen(c, x - 16, y, xv, yv));
					}
				}
		};
};

class ProjectileYellow : public Projectile {
	public:
		ProjectileYellow(Context *c, int x, int y, int xv, int yv) : Projectile(c, x, y, 8, 8, xv, yv, 4, new ImageResource("textures/bullet_yellow.png")) {
			YELLOW_FIRE->play();
		}
		void collide() {
			for (Entity *e : contacts) {
				if (e != NULL) {
					if (EnemyShip *es = dynamic_cast<EnemyShip *>(e)) {
						if (es->type == "green") {
							lifespan = 0;
							continue;
						} else if (es->type == "yellow") {
							int r = rand() % 4;
							switch (r) {
								case 0:
									ENTITY_MANAGER->add_entity(new ProjectileGreen(c, es->get_x() - 40, es->get_y(), -512, 0));
									break;
								case 1:
									ENTITY_MANAGER->add_entity(new ProjectileGreen(c, es->get_x() + 40, es->get_y(), 512, 0));
									break;
								case 2:
									ENTITY_MANAGER->add_entity(new ProjectileGreen(c, es->get_x(), es->get_y() - 40, 0, -512));
									break;
								case 3:
									ENTITY_MANAGER->add_entity(new ProjectileGreen(c, es->get_x(), es->get_y() + 40, 0, 512));
									break;
							}
						}
					}
					lifespan = 0;
					if (e->is_alive()) {
						e->take_damage(strength);
					}
				}
			}
		}
		class Port : public WeaponPort {
			public:
				Port() : firing(false), timer(0) {}
				void update() {
					if (firing) {
						timer = (timer + 1) % 5;
						if (timer == 0) {
							if (xv < 0) {
								ENTITY_MANAGER->add_entity(new ProjectileYellow(c, PLAYER->get_x() - 40, PLAYER->get_y(), xv, yv));
							} else if (xv > 0) {
								ENTITY_MANAGER->add_entity(new ProjectileYellow(c, PLAYER->get_x() + 40, PLAYER->get_y(), xv, yv));
							}
							if (yv < 0) {
								ENTITY_MANAGER->add_entity(new ProjectileYellow(c, PLAYER->get_x(), PLAYER->get_y() - 40, xv, yv));
							} else if (yv > 0) {
								ENTITY_MANAGER->add_entity(new ProjectileYellow(c, PLAYER->get_x(), PLAYER->get_y() + 40, xv, yv));
							}
						}
					}
				}
				void fire(int px, int py, int pxv, int pyv) {
					firing = true;
					xv = pxv;
					yv = pyv;
				}
				void stop() {
					firing = false;
				}
			private:
				bool firing;
				int xv, yv;
				int timer;
		};
};

class ProjectileRed : public Projectile {
	public:
		ProjectileRed(Context *c, int x, int y, int xv, int yv) : Projectile(c, x + rand() % 8 - 4, y + rand() % 8 - 4, 8, 8, xv, yv, 1, new ColorResource(rand() % 16 + 8, rand() % 16 + 8, (float) (rand() % 100 + 50) / 100.0, 0.0, 0.0, 1.0)) {}
		void collide() {
			for (Entity *e : contacts) {
				if (e != NULL) {
					if (EnemyShip *es = dynamic_cast<EnemyShip *>(e)) {
						if (es->type == "green") {
							lifespan = 0;
							continue;
						}
					}
					if (e->is_alive()) {
						e->take_damage(strength);
					}
				}
				lifespan = 0;
			}
		}
		class Port : public WeaponPort {
			public:
				Port() : firing(false), timer(0) {}
				void update() {
					if (firing) {
						timer = (timer + 1) % 5;
						if (timer == 0) {
							if (xv < 0) {
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x() - 45, PLAYER->get_y() + 16, xv, yv + rand() % 100));
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x() - 45, PLAYER->get_y(), xv, yv));
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x() - 45, PLAYER->get_y() - 16, xv, yv - rand() % 100));
							} else if (xv > 0) {
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x() + 45, PLAYER->get_y() + 16, xv, yv + rand() % 100));
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x() + 45, PLAYER->get_y(), xv, yv));
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x() + 45, PLAYER->get_y() - 16, xv, yv - rand() % 100));
							}
							if (yv < 0) {
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x() + 16, PLAYER->get_y() - 45, xv + rand() % 100, yv));
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x(), PLAYER->get_y() - 45, xv, yv));
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x() - 16, PLAYER->get_y() - 45, xv - rand() % 100, yv));
							} else if (yv > 0) {
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x() + 16, PLAYER->get_y() + 45, xv, yv + rand() % 100));
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x(), PLAYER->get_y() + 45, xv, yv));
								ENTITY_MANAGER->add_entity(new ProjectileRed(c, PLAYER->get_x() - 16, PLAYER->get_y() + 45, xv, yv - rand() % 100));
							}
						}
					}
				}
				void fire(int px, int py, int pxv, int pyv) {
					RED_FIRE->loop();
					firing = true;
					xv = pxv;
					yv = pyv;
				}
				void stop() {
					RED_FIRE->halt();
					firing = false;
				}
			private:
				bool firing;
				int xv, yv;
				int timer;
		};
};

class ProjectileBlue : public Projectile {
	public:
		ProjectileBlue(Context *c, int x, int y, int xv, int yv) : Projectile(c, x, y, 32, 32, xv, yv, 100, new ImageResource("textures/bullet_blue.png")) {
			BLUE_FIRE->play();
		}
		void collide() {
			for (Entity *e : contacts) {
				if (e != NULL) {
					if (EnemyShip *es = dynamic_cast<EnemyShip *>(e)) {
						if (es->type == "green") {
							lifespan = 0;
							continue;
						}
					}
					if (e->is_alive()) {
						e->take_damage(strength);
					}
				}
				lifespan = 0;
			}
		}
		void draw(Context *c, int delta) {
			b2Vec2 position = get_body()->GetPosition();
			if (get_xv() < 0) {
				resource->draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16, 90);
			} else if (get_xv() > 0) {
				resource->draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16, 270);
			}
			if (get_yv() < 0) {
				resource->draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16, 0);
			} else if (get_yv() > 0) {
				resource->draw(c, (position.x - get_w()) * 16, (position.y - get_h()) * 16, 180);
			}
		}
		class Port : public WeaponPort {
			public:
				void fire(int x, int y, int xv, int yv) {
					if (xv < 0) { ENTITY_MANAGER->add_entity(new ProjectileBlue(c, x - 24, y, 0, 256));
					} else if (xv > 0) {
						ENTITY_MANAGER->add_entity(new ProjectileBlue(c, x + 24, y, 0, -256));
					}
					if (yv < 0) {
						ENTITY_MANAGER->add_entity(new ProjectileBlue(c, x, y - 24, -256, 0));
					} else if (yv > 0) {
						ENTITY_MANAGER->add_entity(new ProjectileBlue(c, x, y + 24, 256, 0));
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
		case SDLK_x:
			if (e.type == SDL_KEYDOWN) {
				if (TELEPORT_INDEX >= 0) {
					BLOOP->play();
					PLAYER->warp(16, 200);
					TELEPORTS[TELEPORT_INDEX--]->destroy();
				}
			}
			break;
	}
}

void switch_wave() {
	ENTITY_MANAGER->reset_entities(c);
	NUM_SHIPS = 0;
	if (PLAYER) {
		PLAYER->weapons[0]->stop();
		PLAYER->weapons[1]->stop();
		PLAYER->weapons[2]->stop();
		PLAYER->weapons[3]->stop();
		SUCCESS[rand() % 2]->play();
		TIME += 10000 > TIME ? TIME : 10000;
		WAVE += 1;
	}
	PLAYER = new PlayerShip(c, 16, 200);
	PLAYER->weapons[0] = new ProjectileGreen::Port();
	PLAYER->weapons[1] = new ProjectileYellow::Port();
	PLAYER->weapons[2] = new ProjectileRed::Port();
	PLAYER->weapons[3] = new ProjectileBlue::Port();
	ENTITY_MANAGER->add_entity(PLAYER);

	string colors[5] = {"white", "green", "yellow", "red", "blue"};
	
	for (int x = -400; x < 400; x += 40) {
		for (int y = -400; y < -200; y += 40) {
			ENTITY_MANAGER->add_entity(new EnemyShip(c, x, y, colors[rand() % 5]));
		}
	}
}

void start_main_game() {
	ENTITY_MANAGER = new EntityManager(c);
	switch_wave();

	r->add_module(ENTITY_MANAGER);

	CHARGES = new GuiElement(c, 0, 0, (16 * 20), 16, new TextResource("Teleports Remaining:", 2));
	GUI_MANAGER->add_gui_element(CHARGES);
	TELEPORTS[0] = new GuiElement(c, (16 * 20), 0, 16, 16, new ImageResource("textures/battery.png"));
	GUI_MANAGER->add_gui_element(TELEPORTS[0]);
	TELEPORTS[1] = new GuiElement(c, (16 * 20) + 20, 0, 16, 16, new ImageResource("textures/battery.png"));
	GUI_MANAGER->add_gui_element(TELEPORTS[1]);
	TELEPORTS[2] = new GuiElement(c, (16 * 20) + 40, 0, 16, 16, new ImageResource("textures/battery.png"));
	GUI_MANAGER->add_gui_element(TELEPORTS[2]);

	TIME_TEXT = new GuiElement(c, 0, 32, (16 * 15), 16, new TextResource("Time Remaining:", 2));
	GUI_MANAGER->add_gui_element(TIME_TEXT);

	TIME_NUMBER = new GuiElement(c, (16 * 15), 32, 16, 16, new TextResource("0", 2));
	GUI_MANAGER->add_gui_element(TIME_NUMBER);

	SWITCHER = new Switcher();
	r->add_module(SWITCHER);
	r->add_handler(SDL_KEYDOWN, main_key_handler);
	r->add_handler(SDL_KEYUP, main_key_handler);
}

void game_over() {
	r->remove_module(ENTITY_MANAGER);
	r->remove_module(SWITCHER);
	r->remove_handler(SDL_KEYDOWN);
	r->remove_handler(SDL_KEYUP);

	GAME_OVER = new GuiElement(c, c->get_screen_width()/2 - (9 * 32), c->get_screen_height()/2 - 32, 8, 8, new TextResource("Game Over", 8));
	GUI_MANAGER->add_gui_element(GAME_OVER);

	string waves = "You defeated " + to_string(WAVE) + " waves";
	WAVES_CLEARED = new GuiElement(c, c->get_screen_width()/2 - (waves.length() * 16), c->get_screen_height()/2 + 96, 8, 8, new TextResource(waves, 4));
	GUI_MANAGER->add_gui_element(WAVES_CLEARED);
}

class GuiElementButton : public GuiElement {
	public:
		GuiElementButton(Context *c, int x, int y, int w, int h, Resource *r) : GuiElement(c, x, y, w, h, r) {}
		void on_left_click() {
			TITLE->destroy();
			YOGO->destroy();
			destroy();
			start_main_game();
		}
};

int main(int argc, char *argv[]) {
	srand(time(NULL));
	c = new Context("Midnight Memories", 30.0);
	r = new Reactor(c);

	GREEN_FIRE = new Sound("sounds/green_fire.wav");
	YELLOW_FIRE = new Sound("sounds/green_fire.wav");
	RED_FIRE = new Sound("sounds/red_fire.wav");
	BLUE_FIRE = new Sound("sounds/green_fire.wav");
	BLIP = new Sound("sounds/blip.wav");
	BLOOP = new Sound("sounds/bloop.wav");
	SUCCESS[0] = new Sound("sounds/success.wav");
	SUCCESS[1] = new Sound("sounds/victory.wav");

	GUI_MANAGER = new GuiManager();

	TITLE = new GuiElement(c, c->get_screen_width()/2 - (17 * 32), c->get_screen_height()/2 - 32, 8, 8, new TextResource("Midnight Memories", 8));
	GUI_MANAGER->add_gui_element(TITLE);
	YOGO = new GuiElement(c, c->get_screen_width()/2 - (28 * 16), c->get_screen_height()/2 + 40, 8, 8, new TextResource("You only get one (direction)", 4));
	GUI_MANAGER->add_gui_element(YOGO);

	BUTTON_ENTER = new GuiElementButton(c, c->get_screen_width()/2 - (19 * 16), c->get_screen_height()/2 + 100, (19 * 32), 32, new TextResource("Click here to begin", 4));
	GUI_MANAGER->add_gui_element(BUTTON_ENTER);

	MusicManager music("music/chippy.wav");

	r->add_module(GUI_MANAGER);
	r->add_handler(SDL_MOUSEBUTTONDOWN, [&] (Reactor &r, SDL_Event e) {GUI_MANAGER->handler(r, e);});
	r->run();
	return 0;
}
