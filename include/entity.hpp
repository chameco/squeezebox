#ifndef SQUEEZEBOX_ENTITY_HPP
#define SQUEEZEBOX_ENTITY_HPP

#include <iostream>
#include <string>

#include "image_resource.hpp"
#include "context.hpp"
#include "sound.hpp"

using namespace std;

namespace squeezebox {
	class Entity  {
		public:
			Entity(const Context &c, int x, int y, int iw, int ih, const string path);

			void collide(const Entity &e) {}
			void update();

			void draw(const Context &c, int delta);

			void impulse_x(int i);
			void impulse_y(int i);

			void set_x_velocity(int v);
			void set_y_velocity(int v);
		private:
			class EntityContactListener : public b2ContactListener {
				public:
					EntityContactListener(Entity *e) : entity(e) {}
					void BeginContact(b2Contact *contact) {entity->sound.play();}
					void EndContact(b2Contact *contact) {}
					void PreSolve(b2Contact *contact, const b2Manifold *old_manifold) {}
					void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {}
				private:
					Entity *entity;
			};
			float w, h;
			float xv, yv;
			b2BodyDef body_def;
			b2Body *body;
			b2PolygonShape box;
			b2FixtureDef fixture_def;
			ImageResource resource;
			EntityContactListener listener;
		public:
			Sound sound;
	};
}

#endif
