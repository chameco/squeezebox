#include "entity_manager.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <Box2D/Box2D.h>

#include "context.hpp"
#include "module.hpp"
#include "entity.hpp"

using namespace squeezebox;
using namespace std;

void EntityManager::update(const Context &c) {
	for (Entity *e : all) {
		e->update();
	}
}

void EntityManager::draw(const Context &c, int delta) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glTranslatef(c.get_screen_width()/2 - c.get_camera_x(),
			c.get_screen_height()/2 - c.get_camera_y(), 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (Entity *e : all) {
		e->draw(c, delta);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void EntityManager::add_entity(Entity *e) {
	all.push_back(e);
}
