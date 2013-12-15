#include "sound.hpp"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace squeezebox;
using namespace std;

Sound::Sound(string path) : channel(-1) {
	sound = Mix_LoadWAV(path.c_str());
}

void Sound::play() {
	channel = Mix_PlayChannel(-1, sound, 0);
}

void Sound::loop() {
	channel = Mix_PlayChannel(-1, sound, -1);
}

void Sound::halt() {
	if (channel != -1) {
		Mix_HaltChannel(channel);
		channel = -1;
	}
}
