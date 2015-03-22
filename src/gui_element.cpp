#include "gui_element.hpp"

#include <string>

#include "resource.hpp"
#include "context.hpp"
#include "aabb.hpp"

using namespace squeezebox;
using namespace std;

GuiElement::GuiElement(Context &c, int x, int y, int w, int h, Resource &r)
	: AABB(x, y, w, h), resource(r), alive(true) {
	}

void GuiElement::draw(Context &c, int delta) {
	resource.draw(c, x, y);
}
