#include "gui_element.hpp"

#include <string>

#include "resource.hpp"
#include "context.hpp"
#include "aabb.hpp"

using namespace squeezebox;
using namespace std;

void GuiElement::draw(const Context &c, int delta) {
	resource.draw(c, x, y);
}
