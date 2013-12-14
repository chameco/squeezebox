#include "music_manager.hpp"

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace squeezebox;
using namespace std;

MusicManager::MusicManager(string track_name) : music(NULL) {
	music = Mix_LoadMUS(track_name.c_str());
	Mix_PlayMusic(music, -1);
}

MusicManager::~MusicManager() {
	Mix_HaltMusic();
	Mix_FreeMusic(music);
}
