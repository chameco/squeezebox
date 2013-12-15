#ifndef SQUEEZEBOX_ENTITY_MANAGER_HPP
#define SQUEEZEBOX_ENTITY_MANAGER_HPP

#include <string>
#include <list>
#include <functional>

#include <Box2D/Box2D.h>

#include "context.hpp"
#include "module.hpp"
#include "entity.hpp"

using namespace std;

namespace squeezebox {
	class EntityManager : public Module {
		public:
			EntityManager(Context *c);

			void load_entities(Context *c, string path);
			void load_entities(Context *c, function<Entity *(Context *, int, int, int, int, int, string)> alloc, string path);
			void reset_entities(Context *c);

			void update(Context *c);
			void draw(Context *c, int delta);

			void add_entity(Entity *e);
		private:
			class EntityContactListener : public b2ContactListener {
				public:
					EntityContactListener() {}
					void BeginContact(b2Contact *contact) {
						Entity *a = (Entity *) (contact->GetFixtureA()->GetBody()->GetUserData());
						Entity *b = (Entity *) (contact->GetFixtureB()->GetBody()->GetUserData());
						if (a != NULL) {
							a->add_contact(b);
						}
						if (b != NULL) {
							b->add_contact(a);
						}
					}
					void EndContact(b2Contact *contact) {}
					void PreSolve(b2Contact *contact, const b2Manifold *old_manifold) {}
					void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {}
			};
			EntityContactListener listener;
			list<Entity *> all;
	};
}

#endif
