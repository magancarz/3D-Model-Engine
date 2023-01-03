#pragma once

#include <memory>
#include <vector>

#include "ContrastChanger.h"
#include "HorizontalBlur.h"
#include "VerticalBlur.h"
#include "BrightFilter.h"
#include "CombineFilter.h"
#include "models/RawModel.h"
#include "renderEngine/Loader.h"

class PostProcessing {
public:
	static void post_processing_init(const std::shared_ptr<Loader>& loader);

	static void post_processing_draw(unsigned int color_texture, unsigned int bright_texture);

	static void post_processing_start();

	static void post_processing_end();

private:
	inline static const std::vector<float> post_processing_positions = {
		-1,  1,
		-1, -1,
		 1,  1,
		 1, -1 };

	inline static std::unique_ptr<ContrastChanger> post_processing_contrast_changer;
	inline static std::unique_ptr<HorizontalBlur> post_processing_horizontal_blur;
	inline static std::unique_ptr<VerticalBlur> post_processing_vertical_blur;
	inline static std::unique_ptr<BrightFilter> post_processing_bright_filter;
	inline static std::unique_ptr<CombineFilter> post_processing_combine_filter;

	inline static RawModel* post_processing_quad;
};



