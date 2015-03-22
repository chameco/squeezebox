#ifndef SQUEEZEBOX_ENTITY_MANAGER_HPP
#define SQUEEZEBOX_ENTITY_MANAGER_HPP

#include <string>
#include <list>
#include <functional>
#include <memory>

#include <Box2D/Box2D.h>

#include "context.hpp"
#include "module.hpp"
#include "entity.hpp"

namespace squeezebox {
	class EntityManager : public Module {
		public:
			EntityManager(Context &c);

			void update(Context &c);
			void draw(Context &c, int delta);
			void reset(Context &c);

			void add_entity(Entity &e);
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
			std::list<std::reference_wrapper<Entity>> all;
	};
}

#endif
