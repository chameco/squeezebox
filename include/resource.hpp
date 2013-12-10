#ifndef SQUEEZEBOX_RESOURCE_HPP
#define SQUEEZEBOX_RESOURCE_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "context.hpp"

using namespace std;

namespace squeezebox {
	struct Vertex {
		GLfloat x, y;
		GLfloat s, t;
	};

	class Resource {
		public:
			static GLuint surface_to_texture(SDL_Surface *surface);

			virtual void draw(const Context &c, int x, int y) =0;
	};
}

#endif
