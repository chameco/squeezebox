#ifndef SQUEEZEBOX_IMAGE_RESOURCE_HPP
#define SQUEEZEBOX_IMAGE_RESOURCE_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "context.hpp"
#include "resource.hpp"

using namespace std;

namespace squeezebox {
	class ImageResource : public Resource {
		public:
			static unordered_map<string, ImageResource *> cache;

			ImageResource(string path);
			~ImageResource();

			void draw(const Context &c, int x, int y);
		private:
			void generate_vertices(int w, int h);
			GLuint texture;
			Vertex vertices[4];
			GLuint vertex_handler;
	};
}

#endif
