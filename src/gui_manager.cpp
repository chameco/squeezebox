#include "gui_manager.hpp"

#include <string>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include "context.hpp"
#include "module.hpp"
#include "gui_element.hpp"
#include "reactor.hpp"

using namespace squeezebox;
using namespace std;

void GuiManager::handler(Reactor &r, SDL_Event e) {
	if (e.button.button == SDL_BUTTON_LEFT) {
		left_click(e.button.x, e.button.y);
	} else if (e.button.button == SDL_BUTTON_RIGHT) {
		right_click(e.button.x, e.button.y);
	}
}

static bool destroy_gui_element_predicate(GuiElement &e) {
	return !e.is_alive();
}

void GuiManager::update(Context &c) {
	all.remove_if(destroy_gui_element_predicate);
}

void GuiManager::draw(Context &c, int delta) {
	for (GuiElement &e : all) {
		e.draw(c, delta);
	}
}

void GuiManager::reset(Context &c) {
	all.clear();
}

void GuiManager::add_gui_element(GuiElement &e) {
	all.push_back(e);
}

void GuiManager::left_click(int x, int y) {
	const AABB cursor(x, y, 1, 1);
	for (GuiElement &e : all) {
		if (e.collides_with(cursor)) {
			e.on_left_click();
		}
	}
}

void GuiManager::right_click(int x, int y) {
	const AABB cursor(x, y, 1, 1);
	for (GuiElement &e : all) {
		if (e.collides_with(cursor)) {
			e.on_right_click();
		}
	}
}
