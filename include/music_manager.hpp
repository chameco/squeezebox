#ifndef SQUEEZEBOX_MUSIC_MANAGER_HPP
#define SQUEEZEBOX_MUSIC_MANAGER_HPP

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace std;

namespace squeezebox {
	class MusicManager {
		public:
			MusicManager(string track_name);
			~MusicManager();
		private:
			Mix_Music *music;
	};
}

#endif
