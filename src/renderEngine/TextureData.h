#pragma once

class TextureData {
public:
	TextureData(unsigned char* data, const int height, const int width);

	unsigned char* get_data() const;

	int get_width() const;
	int get_height() const;

private:
	unsigned char* m_data;

	int m_width, m_height;
};