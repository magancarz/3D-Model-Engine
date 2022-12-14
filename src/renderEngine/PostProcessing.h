#pragma once

#include "Loader.h"
#include "ContrastChanger.h"
#include "HorizontalBlur.h"
#include "VerticalBlur.h"

const std::vector<float> POST_PROCESSING_POSITIONS = { -1, 1, -1, -1, 1, 1, 1, -1 };

static ContrastChanger* POST_PROCESSING_CONTRAST_CHANGER = nullptr;
static HorizontalBlur* POST_PROCESSING_HORIZONTAL_BLUR = nullptr;
static VerticalBlur* POST_PROCESSING_VERTICAL_BLUR = nullptr;

static RawModel* POST_PROCESSING_QUAD = nullptr;

void POST_PROCESSING_INIT(Loader* loader);

void POST_PROCESSING_DRAW(unsigned int colorTexture);

void POST_PROCESSING_CLEAN_UP();

void POST_PROCESSING_START();

void POST_PROCESSING_END();