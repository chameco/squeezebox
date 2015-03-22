#ifndef SQUEEZEBOX_MODULE_HPP
#define SQUEEZEBOX_MODULE_HPP

#include "context.hpp"

namespace squeezebox {
	class Module {
		public:
			Module() : alive(true) {}
			virtual ~Module() {}
			virtual void update(Context &c) =0;
			virtual void draw(Context &c, int delta) =0;
			virtual void reset(Context &c) =0;
			void destroy() { alive = false; }
			bool is_alive() { return alive; }
		private:
			bool alive;
	};
}

#endif
