#include "world.hpp"

#include <string>
#include <vector>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#include <Box2D/Box2D.h>

#include "module.hpp"
#include "resource.hpp"
#include "vm.hpp"

using namespace std;
using namespace squeezebox;

World::World(Context &c, string path) {
	load_geometry(c, path);
}

void World::load_geometry(Context &c, string path) {
	c.vm.load_file(path);
	int size = c.vm.get_table_size("geometry");
	unsigned int index = 1;
	auto descend_1 = [&](){c.vm.descend_array(index);};
	auto descend_color = [&](){c.vm.descend_array(index); c.vm.descend_table("color");};
	for (; index <= size; ++index) {
		WorldElement w;
		w.x = c.vm.get_numeric("x", "geometry", descend_1)/16.0;
		w.y = c.vm.get_numeric("y", "geometry", descend_1)/16.0;
		w.w = c.vm.get_numeric("w", "geometry", descend_1)/32.0;
		w.h = c.vm.get_numeric("h", "geometry", descend_1)/32.0;
		w.r = c.vm.get_numeric("r", "geometry", descend_color);
		w.g = c.vm.get_numeric("g", "geometry", descend_color);
		w.b = c.vm.get_numeric("b", "geometry", descend_color);
		/*string tpath = c.vm.get_string("texture", "geometry", descend_1);
		if (tpath != "") {
			SDL_Surface *surface = IMG_Load(tpath.c_str());
			if (surface == NULL) {
				cerr << "File " << tpath.c_str() << " does not exist" << endl;
				exit(1);
			}
			w.texture = Resource::surface_to_texture(surface);
		} else {
			w.texture = 0;
		}
		w.body_def.position.Set(w.x, w.y);
		w.body = c.world.CreateBody(&(w.body_def));
		w.geometry.SetAsBox(w.w, w.h);
		w.body->CreateFixture(&w.geometry, 0.0);*/
		elements.push_back(w);
	}
}

void World::draw(Context &c, int delta) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glTranslatef(c.get_screen_width()/2 - c.get_camera_x(),
			c.get_screen_height()/2 - c.get_camera_y(), 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (WorldElement e : elements) {
		glPushMatrix();

		glTranslatef((e.x - e.w) * 16.0, (e.y - e.h) * 16.0, 0);
		glColor3f(e.r, e.g, e.b);
		glBindTexture(GL_TEXTURE_2D, e.texture);

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

void World::reset(Context &c) {
	elements.clear();
}
