#ifndef SQUEEZEBOX_PROJECTILE_HPP
#define SQUEEZEBOX_PROJECTILE_HPP

#include <string>

#include "context.hpp"
#include "entity.hpp"

namespace squeezebox {
	class Projectile : public Entity {
		public:
			Projectile(Context &c, int x, int y, int iw, int ih, int xv, int yv, int strength, Resource &r);

			void update();
			void collide();
		protected:
			int lifespan;
			int strength;
	};
}

#endif
