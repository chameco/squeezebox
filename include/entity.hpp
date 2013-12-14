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
			Entity(Context *c, int x, int y, int iw, int ih, const string path);
			virtual ~Entity() {}

			virtual void collide() {}
			virtual void update();
			
			void destroy();
			bool is_alive();

			void draw(Context *c, int delta);

			int get_x() { return body->GetPosition().x * 16; }
			int get_y() { return body->GetPosition().y * 16; }
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
			ImageResource resource;
			bool alive;
		protected:
			list<Entity *> contacts;
	};
}

#endif
