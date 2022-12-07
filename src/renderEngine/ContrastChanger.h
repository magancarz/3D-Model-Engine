#pragma once

#include "../shaders/ContrastShader.h"
#include "ImageRenderer.h"

class ContrastChanger {
public:
	ContrastChanger();

	void render(unsigned int texture);

	void cleanUp();

private:
	ImageRenderer* m_renderer;
	ContrastShader* m_shader;
};