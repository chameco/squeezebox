#include "resource.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace squeezebox;
using namespace std;

static GLuint surface_to_texture(SDL_Surface *surface) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int mode;
	if (surface->format->BytesPerPixel == 4) {
		if (surface->format->Rmask == 0x000000ff) {
			mode = GL_RGBA;
		} else {
			mode = GL_BGRA;
		}
	}
	else {
		if (surface->format->Rmask == 0x000000ff) {
			mode = GL_RGB;
		} else {
			mode = GL_BGR;
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, surface->h, 0,
			mode, GL_UNSIGNED_BYTE, surface->pixels);
	SDL_FreeSurface(surface);
	return texture;
}

Resource::Resource(string path) : texture(0), vertex_handler(0) {
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (surface == NULL) {
		cerr << "File " << path << " does not exist" << endl;
		exit(1);
	}
	int w = surface->w;
	int h = surface->h;
	texture = surface_to_texture(surface);
	vertices[0].x = 0;
	vertices[0].y = 0;

	vertices[1].x = w;
	vertices[1].y = 0;

	vertices[2].x = w;
	vertices[2].y = h;

	vertices[3].x = 0;
	vertices[3].y = h;

	vertices[0].s = 0;
	vertices[0].t = 0;

	vertices[1].s = 1;
	vertices[1].t = 0;

	vertices[2].s = 1;
	vertices[2].t = 1;

	vertices[3].s = 0;
	vertices[3].t = 1;

	glGenBuffers(1, &vertex_handler);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_handler);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
}

Resource::~Resource() {
	glDeleteTextures(1, &texture);
}

void Resource::draw(const Context &c, int x, int y) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_handler);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)(sizeof(GLfloat)*2));
		glVertexPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c.get_standard_indices_handler());
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}
