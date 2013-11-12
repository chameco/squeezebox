#ifndef SQUEEZEBOX_AABB_HPP
#define SQUEEZEBOX_AABB_HPP

namespace squeezebox {
	class AABB {
		public:
			AABB(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
			bool collides_with(const AABB &other);
		protected:
			int x, y;
			int w, h;
	};
}

#endif
