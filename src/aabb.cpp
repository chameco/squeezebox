#include "aabb.hpp"

using namespace squeezebox;

bool AABB::collides_with(const AABB &other) {
	return (((x >= other.x && x < other.x + other.w) ||
				(other.x >= x && other.x < x + w)) && 
			((y >= other.y && y < other.y + other.h) ||
			 (other.y >= y && other.y < y + h)));
}
