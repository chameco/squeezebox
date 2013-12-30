#ifndef SQUEEZEBOX_COLOR_RESOURCE_HPP
#define SQUEEZEBOX_COLOR_RESOURCE_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "context.hpp"
#include "resource.hpp"

using namespace std;

namespace squeezebox {
	class ColorResource : public Resource {
		public:
			ColorResource(int w, int h, double r, double g, double b);
			void draw(Context *c, int x, int y, int rotation=0);
		private:
			double r, g, b;
			Vertex vertices[4];
			GLuint vertex_handler;
	};
}

#endif
