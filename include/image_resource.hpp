#ifndef SQUEEZEBOX_IMAGE_RESOURCE_HPP
#define SQUEEZEBOX_IMAGE_RESOURCE_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "context.hpp"
#include "resource.hpp"

namespace squeezebox {
	class ImageResource : public Resource {
		public:
			ImageResource(std::string path);

			~ImageResource();

			void draw(Context &c, int x, int y);
		private:
			void generate_vertices(int w, int h);
			GLuint texture;
			GLuint vertex_handler;
	};
}

#endif
