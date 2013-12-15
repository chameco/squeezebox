#ifndef SQUEEZEBOX_ENTITY_HPP
#define SQUEEZEBOX_ENTITY_HPP

#include <iostream>
#include <string>
#include <list>

#include "image_resource.hpp"
#include "context.hpp"
#include "sound.hpp"

using namespace std;

namespace squeezebox {
	class Entity  {
		public:
			Entity(int iw, int ih, int hp, Resource *r) : w(iw/32.0f), h(ih/32.0f), xv(0), yv(0), alive(true), hp(hp), resource(r) {}
			Entity(Context *c, int x, int y, int iw, int ih, int hp, Resource *r);
			virtual ~Entity();

			virtual void collide() {}
			virtual void update();

			virtual void draw(Context *c, int delta);

			virtual void take_damage(int d);
			
			virtual void destroy() { alive = false; }
			bool is_alive() { return alive; }

			void warp(int x, int y);

			int get_x() { return body->GetPosition().x * 16; }
			int get_y() { return body->GetPosition().y * 16; }
			int get_w() { return w; }
			int get_h() { return h; }
			int get_xv() { return xv; }
			int get_yv() { return yv; }
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
			b2BodyDef body_def;
			b2Body *body;
			b2PolygonShape box;
			b2FixtureDef fixture_def;
			Resource *resource;
			list<Entity *> contacts;
	};
}

#endif
