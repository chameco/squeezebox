#ifndef SQUEEZEBOX_GUI_ELEMENT_HPP
#define SQUEEZEBOX_GUI_ELEMENT_HPP

#include <string>

#include "resource.hpp"
#include "context.hpp"
#include "aabb.hpp"

using namespace std;

namespace squeezebox {
	class GuiElement : public AABB {
		public:
			GuiElement(const Context &c, int x, int y, int w, int h, const string path) : AABB(x, y, w, h), resource(path) {}

			virtual void on_left_click() =0;
			virtual void on_right_click() =0;

			void draw(const Context &c, int delta);
		private:
			Resource resource;
	};
}

#endif
