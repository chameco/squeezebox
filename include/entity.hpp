#ifndef SQUEEZEBOX_ENTITY_HPP
#define SQUEEZEBOX_ENTITY_HPP

#include <iostream>
#include <string>
#include <list>

#include "resource.hpp"
#include "context.hpp"

namespace squeezebox {
	class Entity {
		public:
			Entity(Context &c, int x, int y, int iw, int ih, int hp, Resource &r);

			virtual void collide() {}
			virtual void update();

			virtual void draw(Context &c, int delta);

			virtual void take_damage(const int d);
			
			virtual void destroy() { alive = false; }
			const bool is_alive() const { return alive; }

			void warp(int x, int y);

			const int get_x() const { return body->GetPosition().x * 16; }
			const int get_y() const { return body->GetPosition().y * 16; }
			const int get_w() const { return w; }
			const int get_h() const { return h; }
			const int get_xv() const { return xv; }
			const int get_yv() const { return yv; }
			b2Body *get_body() { return body; }

			void impulse_x(int i);
			void impulse_y(int i);

			void set_x_velocity(int v);
			void set_y_velocity(int v);

			void add_contact(Entity *e);
			void remove_contact(Entity *e);
		protected:
			float w, h;
			float xv, yv;
			bool alive;
			int hp;
			b2Body *body;
			b2PolygonShape box;
			b2FixtureDef fixture_def;
			Resource &resource;
			std::list<Entity *> contacts;
	};
}

#endif
