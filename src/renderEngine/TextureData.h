#pragma once

class TextureData {
public:
	TextureData(unsigned char* data, int height, int width)
		: m_data(data), m_height(height), m_width(width) {}

	inline unsigned char* getData() { return m_data; };

	inline int getWidth() { return m_width; };
	inline int getHeight() { return m_height; };
private:
	unsigned char* m_data;

	int m_width, m_height;
};