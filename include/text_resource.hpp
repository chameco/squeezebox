#ifndef SQUEEZEBOX_TEXT_RESOURCE_HPP
#define SQUEEZEBOX_TEXT_RESOURCE_HPP

#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "resource.hpp"

namespace squeezebox {
	class TextResource : public Resource {
		public:
			static SDL_Surface *bitmap;
			static GLuint bitmap_font[4][26];
			static Vertex glyph[4];
			static GLuint glyph_handler;

			TextResource(std::string t, double s);

			void draw(Context &c, int x, int y);
			void set_text(std::string t) { text = t; }
		private:
			std::string text;
			double size;
	};
}

#endif
