#pragma once

class TextureData {
public:
	TextureData(unsigned char* data, const int height, const int width) :
	m_data(data), m_width(height), m_height(width) {}

	unsigned char* get_data() const { return m_data; }

	int get_width() const { return m_width; }
	int get_height() const { return m_height; }

private:
	unsigned char* m_data;

	int m_width, m_height;
};