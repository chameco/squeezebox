#include "projectile.hpp"

#include <string>

#include "context.hpp"
#include "entity.hpp"
#include "sound.hpp"

using namespace squeezebox;
using namespace std;

Projectile::Projectile(Context *c, int x, int y, int iw, int ih, int xv, int yv, int strength, string path) : Entity(c, x, y, iw, ih, 1, path),
lifespan(256), strength(strength) {
	set_x_velocity(xv);
	set_y_velocity(yv);
	get_body()->SetGravityScale(0);
}

void Projectile::update() {
	lifespan -= 1;
	Entity::update();
	if (lifespan <= 0) {
		destroy();
	}
}

void Projectile::collide() {
	for (Entity *e : contacts) {
		if (e != NULL) {
			e->take_damage(strength);
		}
		lifespan = 0;
	}
}
