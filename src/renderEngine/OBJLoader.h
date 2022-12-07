#pragma once

#include "../Headers.h"

#include "../renderEngine/Loader.h"
#include "../models/RawModel.h"
#include "Vertex.h"

RawModel* loadOBJ(const std::string& fileName, Loader* loader);