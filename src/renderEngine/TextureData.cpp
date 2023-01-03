#include "TextureData.h"

TextureData::TextureData(unsigned char* data, const int height, const int width) :
m_data(data), m_width(height), m_height(width) {}

unsigned char* TextureData::get_data() const {
	return m_data;
}

int TextureData::get_width() const {
	return m_width;
}

int TextureData::get_height() const {
	return m_height;
}