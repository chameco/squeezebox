#ifndef SQUEEZEBOX_GUI_ELEMENT_HPP
#define SQUEEZEBOX_GUI_ELEMENT_HPP

#include <string>
#include <memory>
#include <cstdio>

#include "resource.hpp"
#include "context.hpp"
#include "aabb.hpp"

namespace squeezebox {
	class GuiElement : public AABB {
		public:
			GuiElement(Context &c, int x, int y, int w, int h, Resource &r);

			void destroy() { alive = false; }
			bool is_alive() { return alive; }

			virtual void on_left_click() {}
			virtual void on_right_click() {}

			void draw(Context &c, int delta);
		private:
			Resource &resource;
			bool alive;
	};
}

#endif
