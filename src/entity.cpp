#include "entity.hpp"

#include <string>

#include "resource.hpp"
#include "context.hpp"

using namespace squeezebox;
using namespace std;

void Entity::warp(int x, int y) {
	this->x = x;
	this->y = y;
}

void Entity::set_velocity(int x_velocity, int y_velocity) {
	this->x_velocity = x_velocity;
	this->y_velocity = y_velocity;
}

void Entity::draw(const Context &c, int delta) {
	resource.draw(c, x, y);
}
