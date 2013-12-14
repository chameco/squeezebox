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
			Entity(Context *c, int x, int y, int iw, int ih, int hp, const string path);
			virtual ~Entity() {}

			virtual void collide() {}
			virtual void update();

			virtual void draw(Context *c, int delta);

			void take_damage(int d);
			
			virtual void destroy() { alive = false; }
			bool is_alive() { return alive; }

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
		private:
			float w, h;
			float xv, yv;
			b2BodyDef body_def;
			b2Body *body;
			b2PolygonShape box;
			b2FixtureDef fixture_def;
			bool alive;
			int hp;
		protected:
			ImageResource resource;
			list<Entity *> contacts;
	};
}

#endif
