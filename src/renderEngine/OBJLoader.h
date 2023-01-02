#pragma once

#include "../renderEngine/Loader.h"
#include "../models/RawModel.h"

RawModel* loadOBJ(const std::string& fileName, Loader* loader);