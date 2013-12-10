#include "sound.hpp"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace squeezebox;
using namespace std;

Sound::Sound(string path) {
	sound = Mix_LoadWAV(path.c_str());
}

void Sound::play() {
	Mix_PlayChannel(-1, sound, 0);
}
