#include "world.hpp"

#include <string>
#include <vector>
#include <fstream>

#ifdef _WIN32
 #include <jsoncpp/json.h>
#else
 #include <jsoncpp/json/json.h>
#endif

#include <Box2D/Box2D.h>

#include "module.hpp"

using namespace std;
using namespace squeezebox;

World::World(Context *c, string path) {
	load_geometry(c, path);
}

void World::load_geometry(Context *c, string path) {
	Json::Value root;
	Json::Reader reader;
	fstream input;
	input.open(path);
	input >> root;
	input.close();
	const Json::Value geometry = root.get("geometry", 0);
	if (geometry.isArray()) {
		for (unsigned int index = 0; index < geometry.size(); ++index) {
			WorldElement w;
			w.x = geometry[index].get("x", 0).asDouble()/16.0;
			w.y = geometry[index].get("y", 0).asDouble()/16.0;
			w.w = geometry[index].get("w", 0).asDouble()/32.0;
			w.h = geometry[index].get("h", 0).asDouble()/32.0;
			w.body_def.position.Set(w.x, w.y);
			w.body = c->get_world()->CreateBody(&(w.body_def));
			w.geometry.SetAsBox(w.w, w.h);
			w.body->CreateFixture(&w.geometry, 0.0);
			elements.push_back(w);
		}
	}
}

void World::draw(Context *c, int delta) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glTranslatef(c->get_screen_width()/2 - c->get_camera_x(),
			c->get_screen_height()/2 - c->get_camera_y(), 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (WorldElement e : elements) {
		glPushMatrix();

		glTranslatef((e.x - e.w) * 16.0, (e.y - e.h) * 16.0, 0);
		glColor3f(1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBegin(GL_QUADS);
			glTexCoord2i(0, 0); glVertex3f(0, 0, 0);
			glTexCoord2i(1, 0); glVertex3f(e.w*32.0, 0, 0);
			glTexCoord2i(1, 1); glVertex3f(e.w*32.0, e.h*32.0, 0);
			glTexCoord2i(0, 1); glVertex3f(0, e.h*32.0, 0);
		glEnd();

		glPopMatrix();
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
