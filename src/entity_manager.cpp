#include "entity_manager.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <functional>

#include <GL/glew.h>
#include <Box2D/Box2D.h>

/*#ifdef _WIN32
 #include <jsoncpp/json.h>
#else
 #include <jsoncpp/json/json.h>
#endif*/

#include "context.hpp"
#include "module.hpp"
#include "entity.hpp"

using namespace squeezebox;
using namespace std;

EntityManager::EntityManager(Context *c) {
	c->get_world()->SetContactListener(&listener);
}

void EntityManager::load_entities(Context *c, string path) {
	load_entities(c, [] (Context *c, int x, int y, int iw, int ih, int hp, string path) { return new Entity(c, x, y, iw, ih, hp, new ImageResource(path)); }, path);
}

void EntityManager::load_entities(Context *c, function<Entity *(Context *, int, int, int, int, int, string)> alloc, string path) {
	/*Json::Value root;
	Json::Reader reader;
	fstream input;
	input.open(path);
	input >> root;
	input.close();
	const Json::Value entities = root.get("entities", 0);
	if (entities.isArray()) {
		for (unsigned int index = 0; index < entities.size(); ++index) {
			int x, y, w, h, hp;
			string path;
			x = entities[index].get("x", 0).asInt();
			y = entities[index].get("y", 0).asInt();
			w = entities[index].get("w", 0).asInt();
			h = entities[index].get("h", 0).asInt();
			hp = entities[index].get("hp", 1).asInt();
			path = entities[index].get("path", 0).asString();
			Entity *e = alloc(c, x, y, w, h, hp, path);
			e->get_body()->SetGravityScale(entities[index].get("gravity", 1).asDouble());
			add_entity(e);
		}
	}*/
}

void EntityManager::reset_entities(Context *c) {
	for (Entity *e : all) {
		c->get_world()->DestroyBody(e->get_body());
		delete e;
	}
	all.clear();
}

class destroy_entity_predicate {
	public:
		destroy_entity_predicate(Context *con) : c(con) {}
		bool operator()(Entity *e) {
			if (!e->is_alive()) {
				c->get_world()->DestroyBody(e->get_body());
				//delete e;
				return true;
			}
			return false;
		}
	private:
		Context *c;
};

void EntityManager::update(Context *c) {
	all.remove_if(destroy_entity_predicate(c));
	for (Entity *e : all) {
		e->update();
	}
}

void EntityManager::draw(Context *c, int delta) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glTranslatef(c->get_screen_width()/2 - c->get_camera_x(),
			c->get_screen_height()/2 - c->get_camera_y(), 0);
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
