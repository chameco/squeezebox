#ifndef SQUEEZEBOX_ENTITY_HPP
#define SQUEEZEBOX_ENTITY_HPP

#include <string>

#include "resource.hpp"
#include "context.hpp"
#include "aabb.hpp"

using namespace std;

namespace squeezebox {
	class Entity : public AABB {
		public:
			Entity(const Context &c, int x, int y, int w, int h, const string path) : AABB(x, y, w, h), resource(path) {}

			virtual void collide(const Entity &e) =0;
			virtual void update() =0;

			void draw(const Context &c, int delta);
			void warp(int x, int y);
			void set_velocity(int x_velocity, int y_velocity);
		private:
			int x_velocity, y_velocity;
			Resource resource;
	};
}

#endif
