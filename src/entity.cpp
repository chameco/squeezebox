#include "entity.hpp"

#include <iostream>
#include <string>

#include "image_resource.hpp"
#include "context.hpp"
#include "sound.hpp"

using namespace squeezebox;
using namespace std;

Entity::Entity(const Context &c, int x, int y, int iw, int ih, const string path)
: w(iw/32.0f), h(ih/32.0f), xv(0), yv(0), resource(path), listener(this), sound("test.wav") {
	body_def.type = b2_dynamicBody;
	body_def.position.Set(x/16.0f, y/16.0f);
	body_def.fixedRotation = true;
	body = c.get_world()->CreateBody(&body_def);
	body->SetUserData((void *) this);
	box.SetAsBox(w, h);
	fixture_def.shape = &box;
	fixture_def.density = 1.4f;
	fixture_def.friction = 0.3f;
	body->CreateFixture(&fixture_def);
	c.get_world()->SetContactListener(&listener);
}

void Entity::update() {
	b2Vec2 vel = body->GetLinearVelocity();
	float x_impulse = 0;
	float y_impulse = 0;
	if (xv != 0) {
		x_impulse = body->GetMass() * (xv/16.0f - vel.x);
	}
	if (yv != 0) {
		y_impulse = body->GetMass() * (yv/16.0f - vel.y);
	}
	body->ApplyLinearImpulse(b2Vec2(x_impulse, y_impulse), body->GetWorldCenter(), true);
}

void Entity::draw(const Context &c, int delta) {
	b2Vec2 position = body->GetPosition();
	resource.draw(c, (position.x - w) * 16, (position.y - h) * 16);
}

void Entity::impulse_x(int i) {
	body->ApplyLinearImpulse(b2Vec2(i/16.0f, 0), body->GetWorldCenter(), true);
}

void Entity::impulse_y(int i) {
	body->ApplyLinearImpulse(b2Vec2(0, i/16.0f), body->GetWorldCenter(), true);
}

void Entity::set_x_velocity(int v) {
	xv = v;
}

void Entity::set_y_velocity(int v) {
	yv = v;
}
