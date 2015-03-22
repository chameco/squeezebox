#ifndef SQUEEZEBOX_GUI_MANAGER_HPP
#define SQUEEZEBOX_GUI_MANAGER_HPP

#include <string>
#include <memory>
#include <list>

#include <SDL2/SDL.h>

#include "context.hpp"
#include "module.hpp"
#include "gui_element.hpp"
#include "reactor.hpp"

namespace squeezebox {
	class GuiManager : public Module {
		public:
			GuiManager() {}

			void handler(Reactor &r, SDL_Event e);

			void update(Context &c);
			void draw(Context &c, int delta);
			void reset(Context &c);

			void add_gui_element(GuiElement &e);
			void left_click(int x, int y);
			void right_click(int x, int y);
		private:
			std::list<std::reference_wrapper<GuiElement>> all;
	};
}

#endif
