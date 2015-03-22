#ifndef SQUEEZEBOX_GRAPHICS_PIPELINE_H
#define SQUEEZEBOX_GRAPHICS_PIPELINE_H

#include <list>
#include <functional>

namespace squeezebox {
	class GraphicsPipeline {
		public:
			GraphicsPipeline() {}
			GraphicsPipeline *append_transform(std::function<void(int w, int h)> f);
			void apply_transforms(int w, int h);
		private:
			std::list<std::function<void(int w, int h)>> transforms;
	};
}

#endif
