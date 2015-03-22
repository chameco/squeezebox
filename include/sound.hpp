#ifndef SQUEEZEBOX_SOUND_HPP
#define SQUEEZEBOX_SOUND_HPP

#include <iostream>
#include <string>

#include <SDL2/SDL_mixer.h>

namespace squeezebox {
	class Sound {
		public:
			Sound(std::string path);

			void play();
			void loop();
			void halt();
		private:
			Mix_Chunk *sound;
			int channel;
	};
}

#endif
