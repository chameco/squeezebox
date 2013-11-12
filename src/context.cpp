#include "context.hpp"

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

using namespace squeezebox;
using namespace std;

Context::Context(const string name) : camera_x(0), camera_y(0) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cerr << "Failed to initialize SDL" << endl;    
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if ((screen = SDL_CreateWindow(name.c_str(),
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					0, 0,
					SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN))
			== NULL ) {
		cerr << "Failed to initialize window" << endl;
		exit(1);
	}
	context = SDL_GL_CreateContext(screen);

	SDL_DisplayMode dmode;
	if (SDL_GetWindowDisplayMode(screen, &dmode) != 0) {
		cerr << "Unable to fetch display mode" << endl;
		exit(1);
	}
	screen_width = dmode.w;
	screen_height = dmode.h;

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		cerr << "Initializing GLEW failed with the following error: " << glewGetErrorString(glewError) << endl;
		exit(1);
	}

	if (!GLEW_VERSION_2_1) {
		cerr << "OpenGL 2.1 not supported" << endl;
		exit(1);
	}

	glViewport(0, 0, screen_width, screen_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screen_width, screen_height, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0, 0, 0, 0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	IMG_Init(IMG_INIT_PNG);

	Mix_OpenAudio(22050, AUDIO_S16, 2, 4096);

	standard_indices[0] = 0;
	standard_indices[1] = 1;
	standard_indices[2] = 2;
	standard_indices[3] = 3;

	glGenBuffers(1, &standard_indices_handler);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, standard_indices_handler);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), standard_indices, GL_STATIC_DRAW);
}

Context::~Context() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(screen);
}

void Context::update_screen() {
	SDL_GL_SwapWindow(screen);
}
