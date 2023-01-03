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
	PostProcessing(const std::shared_ptr<Loader>& loader);

	void draw(unsigned int color_texture, unsigned int bright_texture) const;

	void start() const;

	static void end();

private:
	inline static const std::vector<float> QUAD_POSITIONS = {
		-1,  1,
		-1, -1,
		 1,  1,
		 1, -1 };

	std::unique_ptr<ContrastChanger> m_contrast_changer;
	std::unique_ptr<HorizontalBlur> m_horizontal_blur;
	std::unique_ptr<VerticalBlur> m_vertical_blur;
	std::unique_ptr<BrightFilter> m_bright_filter;
	std::unique_ptr<CombineFilter> m_combine_filter;

	std::shared_ptr<RawModel> m_quad;
};



