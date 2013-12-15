#ifndef SQUEEZEBOX_TEXT_RESOURCE_HPP
#define SQUEEZEBOX_TEXT_RESOURCE_HPP

#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "resource.hpp"

using namespace std;

namespace squeezebox {
	class TextResource : public Resource {
		public:
			static SDL_Surface *bitmap;
			static GLuint bitmap_font[4][26];
			static Vertex glyph[4];
			static GLuint glyph_handler;

			TextResource(string t, double s);
			void draw(Context *c, int x, int y, int rotation=0);
			void set_text(string t) { text = t; }
		private:
			string text;
			double size;
	};
}

#endif
