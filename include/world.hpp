#ifndef SQUEEZEBOX_WORLD_HPP
#define SQUEEZEBOX_WORLD_HPP

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <Box2D/Box2D.h>

#include "module.hpp"
#include "resource.hpp"

namespace squeezebox {
	class World : public Module {
		public:
			World(Context &c, std::string path);
			void load_geometry(Context &c, std::string path);

			void update(Context &c) {}
			void draw(Context &c, int delta);
			void reset(Context &c);
		private:
			struct WorldElement {
				double x, y, w, h;
				int r, g, b;
				GLuint texture;
				b2BodyDef body_def;
				b2Body *body;
				b2PolygonShape geometry;
			};
			vector<WorldElement> elements;
	};
}

#endif
