#include "color_resource.hpp"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "context.hpp"
#include "resource.hpp"

using namespace squeezebox;
using namespace std;

ColorResource::ColorResource(int w, int h, double r, double g, double b) : r(r), g(g), b(b) {
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

void ColorResource::draw(Context *c, int x, int y, int rotation) {
	glPushMatrix();
	glTranslatef(x, y, 0);

	if (rotation != 0) {
		glTranslatef(vertices[2].x/2, vertices[2].y/2, 0);
		glRotatef(rotation, 0, 0, -1);
		glTranslatef(-vertices[2].x/2, -vertices[2].y/2, 0);

	}

	glColor3f(r, g, b);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_handler);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)(sizeof(GLfloat)*2));
	glVertexPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->get_standard_indices_handler());
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}
