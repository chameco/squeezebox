#include "text_resource.hpp"

#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#include "resource.hpp"

using namespace squeezebox;
using namespace std;

SDL_Surface *TextResource::bitmap = NULL;
GLuint TextResource::bitmap_font[4][26] = {{0}};
Vertex TextResource::glyph[4];
GLuint TextResource::glyph_handler = 0;

TextResource::TextResource(string t, double s) : text(t), size(s) {
	if (glyph_handler == 0) {
		bitmap = IMG_Load("fonts/font.png");
		//SDL_SetSurfaceAlphaMod(BITMAP, 0);
		SDL_SetSurfaceBlendMode(bitmap, SDL_BLENDMODE_NONE);
		SDL_SetColorKey(bitmap, SDL_TRUE, SDL_MapRGBA(bitmap->format, 255, 0, 0, 255));
		Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif


		int x, y;
		SDL_Rect srcrect = {0, 0, 8, 8};
		SDL_Rect destrect = {0, 0, 0, 0};
		for (y = 0; y < 4; y++) {
			for (x = 0; x < 26; x++) {
				srcrect.x = srcrect.y = 0;
				srcrect.x = x * 8;
				srcrect.y = y * 8;
				SDL_Surface *temp = SDL_CreateRGBSurface(0, 8, 8, 32,
						rmask, gmask, bmask, amask);
				SDL_Surface *character = SDL_ConvertSurface(temp, bitmap->format, bitmap->flags);
				SDL_Rect all = {0, 0, 8, 8};
				SDL_FillRect(character, &all, SDL_MapRGBA(temp->format, 0, 0, 0, 0));
				SDL_FreeSurface(temp);
				SDL_BlitSurface(bitmap, &srcrect, character, &destrect);
				bitmap_font[y][x] = surface_to_texture(character);
			}
		}

		glyph[0].x = 0;
		glyph[0].y = 0;

		glyph[1].x = 8;
		glyph[1].y = 0;

		glyph[2].x = 8;
		glyph[2].y = 8;

		glyph[3].x = 0;
		glyph[3].y = 8;

		glyph[0].s = 0;
		glyph[0].t = 0;

		glyph[1].s = 1;
		glyph[1].t = 0;

		glyph[2].s = 1;
		glyph[2].t = 1;

		glyph[3].s = 0;
		glyph[3].t = 1;

		glGenBuffers(1, &glyph_handler);
		glBindBuffer(GL_ARRAY_BUFFER, glyph_handler);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), glyph, GL_STATIC_DRAW);
	}
}

static int bitmap_index(char c)
{
	if (isalpha(c)) {
		c = toupper(c);
		return c - 65;
	} else if (isdigit(c)) {
		return (c - 48) + 26;
	} else if (isspace(c)) {
		return 51;
	} else if (ispunct(c)) {
		switch (c) {
			case ',':
				return 39;
				break;
			case '\'':
				return 40;
				break;
			case '"':
				return 41;
				break;
			case ':':
				return 42;
				break;
			case ';':
				return 43;
				break;
			case '_':
				return 44;
				break;
			case '/':
				return 45;
				break;
			case '[':
				return 46;
				break;
			case ']':
				return 47;
				break;
			case '(':
				return 48;
				break;
			case ')':
				return 49;
				break;
			case '*':
				return 50;
				break;
		}
	}
	return 51;
}

void TextResource::draw(Context *c, int x, int y, int rotation) {
	int len = text.length();
	int bmpx = 0; int bmpy = 0;
	int curx = x; int cury = y;
	for (int i = 0; i < len; ++i) {
		bmpx = bmpy = 0;
		bmpx = bitmap_index(text[i]);
		if (text[i] == '\n') {
			curx = x;
			cury += 8 * size;
		} else {
			while (bmpx >= 13) {
				bmpx -= 13;
				bmpy += 1;
			}
			glPushMatrix();

			glTranslatef(curx, cury, 0.0);
			glScalef(size, size, 1.0);
			glColor3f(1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, bitmap_font[bmpy][bmpx]);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, glyph_handler);
			glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)(sizeof(GLfloat)*2));
			glVertexPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->get_standard_indices_handler());
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, NULL);

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			glPopMatrix();
			curx += 8 * size;
		}
	}
}
