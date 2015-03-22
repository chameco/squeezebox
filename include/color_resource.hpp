#ifndef SQUEEZEBOX_COLOR_RESOURCE_HPP
#define SQUEEZEBOX_COLOR_RESOURCE_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "context.hpp"
#include "resource.hpp"

namespace squeezebox {
	class ColorResource : public Resource {
		public:
			ColorResource(int w, int h, double r, double g, double b);

			~ColorResource();

			void draw(Context &c, int x, int y);
		private:
			double r, g, b;
			GLuint vertex_handler;
	};
}

#endif
