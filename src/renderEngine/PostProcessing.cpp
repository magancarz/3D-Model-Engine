#include "PostProcessing.h"
#include "DisplayManager.h"

void POST_PROCESSING_INIT(Loader* loader) {
	POST_PROCESSING_QUAD = loader->loadToVAO(POST_PROCESSING_POSITIONS, 2);
	POST_PROCESSING_CONTRAST_CHANGER = new ContrastChanger();
	POST_PROCESSING_HORIZONTAL_BLUR = new HorizontalBlur(WINDOW_WIDTH, WINDOW_HEIGHT);
	POST_PROCESSING_VERTICAL_BLUR = new VerticalBlur(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void POST_PROCESSING_DRAW(unsigned int colorTexture) {
	POST_PROCESSING_START();
	POST_PROCESSING_HORIZONTAL_BLUR->render(colorTexture);
	POST_PROCESSING_VERTICAL_BLUR->render(POST_PROCESSING_HORIZONTAL_BLUR->getOutputTexture());
	//POST_PROCESSING_CONTRAST_CHANGER->render(colorTexture);
	POST_PROCESSING_END();
}

void POST_PROCESSING_CLEAN_UP() {
	POST_PROCESSING_HORIZONTAL_BLUR->cleanUp();
	delete POST_PROCESSING_HORIZONTAL_BLUR;
	POST_PROCESSING_VERTICAL_BLUR->cleanUp();
	delete POST_PROCESSING_VERTICAL_BLUR;
	delete POST_PROCESSING_QUAD;
	delete POST_PROCESSING_CONTRAST_CHANGER;
}

void POST_PROCESSING_START() {
	glBindVertexArray(POST_PROCESSING_QUAD->getVaoID());
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
}

void POST_PROCESSING_END() {
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}