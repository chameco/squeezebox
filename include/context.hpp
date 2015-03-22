#ifndef SQUEEZEBOX_CONTEXT_HPP
#define SQUEEZEBOX_CONTEXT_HPP

#include <string>
#include <memory>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <Box2D/Box2D.h>

#include "vm.hpp"

namespace squeezebox {
	class Context {
		private:
			b2Vec2 gravity;
		public:
			Context(const std::string name, float g);
			~Context();

			const GLuint get_standard_indices_handler() const { return standard_indices_handler; }
			const int get_screen_width() const { return screen_width; }
			const int get_screen_height() const { return screen_height; }
			const int get_camera_x() const { return camera_x; }
			void set_camera_x(int cx) { camera_x = cx; }
			const int get_camera_y() const { return camera_y; }
			void set_camera_y(int cy) { camera_y = cy; }
			
			void update_physics(double delta);
			void update_screen();

			b2World world;
			VM vm;
		private:
			GLuint standard_indices[4];
			GLuint standard_indices_handler;
			SDL_Window *screen;
			SDL_GLContext context;
			int screen_width, screen_height;
			int camera_x;
			int camera_y;
	};
}

#endif
