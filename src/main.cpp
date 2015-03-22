#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>

#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

#include "reactor.hpp"
#include "module.hpp"
#include "entity.hpp"
#include "projectile.hpp"
#include "entity_manager.hpp"
#include "gui_element.hpp"
#include "gui_manager.hpp"
#include "image_resource.hpp"
#include "text_resource.hpp"
#include "color_resource.hpp"
#include "music_manager.hpp"
#include "world.hpp"

using namespace squeezebox;
using namespace std;

int main(int argc, char *argv[]) {
	Context c("test", 30.0);
	Reactor r(c);

	GuiManager gm;
	TextResource t("test", 8);
	GuiElement title(c, 0, 0, 8, 8, t);
	gm.add_gui_element(title);

	World w(c, "levels/wave0");

	EntityManager em(c);
	ImageResource i("textures/enemy_red.png");
	Entity e(c, 0, -300, 32, 32, 100, i);
	em.add_entity(e);

	r.add_module(gm);
	r.add_module(w);
	r.add_module(em);

	r.run();
	
	c.vm.close();

	return 0;
}
