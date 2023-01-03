#pragma once

#include "ContrastShader.h"
#include "renderEngine/postProcessing/ImageRenderer.h"

class ContrastChanger {
public:
	ContrastChanger();

	void render(unsigned int texture) const;

private:
	std::unique_ptr<ImageRenderer> m_renderer;
	std::unique_ptr<ContrastShader> m_shader;
};
