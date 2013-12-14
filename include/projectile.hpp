#ifndef SQUEEZEBOX_PROJECTILE_HPP
#define SQUEEZEBOX_PROJECTILE_HPP

#include <string>

#include "context.hpp"
#include "entity.hpp"

using namespace std;

namespace squeezebox {
	class Projectile : public Entity {
		public:
			Projectile(Context *c, int x, int y, int iw, int ih, int xv, int yv, string path);

			void update();
			void collide();
		private:
			int lifespan;
	};
}

#endif
