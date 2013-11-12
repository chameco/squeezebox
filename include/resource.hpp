#ifndef SQUEEZEBOX_RESOURCE_HPP
#define SQUEEZEBOX_RESOURCE_HPP

#include <string>
#include <vector>

#include <GL/glew.h>

#include "context.hpp"

using namespace std;

namespace squeezebox {
	class Resource {
		public:
			Resource(string path);
			~Resource();

			void draw(const Context &c, int x, int y);
		private:
			struct Vertex {
				GLfloat x, y;
				GLfloat s, t;
			};
			GLuint texture;
			Vertex vertices[4];
			GLuint vertex_handler;
	};
}

#endif
