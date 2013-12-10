#ifndef SQUEEZEBOX_ENTITY_MANAGER_HPP
#define SQUEEZEBOX_ENTITY_MANAGER_HPP

#include <iostream>
#include <string>
#include <vector>

#include <Box2D/Box2D.h>

#include "context.hpp"
#include "module.hpp"
#include "entity.hpp"

using namespace std;

namespace squeezebox {
	class EntityManager : public Module {
		public:
			EntityManager() {}

			void update(const Context &c);
			void draw(const Context &c, int delta);

			void add_entity(Entity *e);
		private:
			vector<Entity *> all;
	};
}

#endif
