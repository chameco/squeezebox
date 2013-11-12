#ifndef SQUEEZEBOX_MODULE_HPP
#define SQUEEZEBOX_MODULE_HPP

#include "context.hpp"

using namespace std;

namespace squeezebox {
	class Module {
		public:
			virtual void update(const Context &c) =0;
			virtual void draw(const Context &c, int delta) =0;
	};
}

#endif
