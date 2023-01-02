#pragma once

#include "ContrastShader.h"
#include "renderEngine/ImageRenderer.h"

class ContrastChanger {
public:
	ContrastChanger();

	void render(unsigned int texture);

	void cleanUp();

private:
	ImageRenderer* m_renderer;
	ContrastShader* m_shader;
};
