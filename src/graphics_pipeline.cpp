#include "graphics_pipeline.hpp"

#include <list>
#include <functional>

#include "resource.hpp"

using namespace squeezebox;
using namespace std;

GraphicsPipeline *GraphicsPipeline::append_transform(function<void(int w, int h)> f) {
	transforms.push_back(f);
	return this;
}

void GraphicsPipeline::apply_transforms(int w, int h) {
	for (function<void(int w, int h)> f : transforms) {
		f(w, h);
	}
}
