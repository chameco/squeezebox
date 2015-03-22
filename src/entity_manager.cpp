#include "entity_manager.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <functional>

#include <GL/glew.h>
#include <Box2D/Box2D.h>

#include "context.hpp"
#include "module.hpp"
#include "entity.hpp"

using namespace squeezebox;
using namespace std;

EntityManager::EntityManager(Context &c) {
	c.world.SetContactListener(&listener);
}

void EntityManager::reset(Context &c) {
	for (Entity &e : all) {
		c.world.DestroyBody(e.get_body());
	}
	all.clear();
}

class destroy_entity_predicate {
	public:
		destroy_entity_predicate(Context &con) : c(con) {}
		bool operator()(Entity &e) {
			if (!e.is_alive()) {
				c.world.DestroyBody(e.get_body());
				return true;
			}
			return false;
		}
	private:
		Context &c;
};

void EntityManager::update(Context &c) {
	all.remove_if(destroy_entity_predicate(c));
	for (Entity &e : all) {
		e.update();
	}
}

void EntityManager::draw(Context &c, int delta) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glTranslatef(c.get_screen_width()/2 - c.get_camera_x(),
			c.get_screen_height()/2 - c.get_camera_y(), 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (Entity &e : all) {
		e.draw(c, delta);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void EntityManager::add_entity(Entity &e) {
	all.push_back(e);
}
