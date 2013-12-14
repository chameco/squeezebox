#ifndef SQUEEZEBOX_WORLD_HPP
#define SQUEEZEBOX_WORLD_HPP

#include <string>
#include <vector>

#include <Box2D/Box2D.h>

#include "module.hpp"

using namespace std;

namespace squeezebox {

	class World : public Module {
		public:
			World(Context *c, string path);
			void load_geometry(Context *c, string path);

			void update(Context *c) {}
			void draw(Context *c, int delta);
		private:
			struct WorldElement {
				double x, y, w, h;
				b2BodyDef body_def;
				b2Body *body;
				b2PolygonShape geometry;
			};
			vector<WorldElement> elements;
	};
}

#endif
