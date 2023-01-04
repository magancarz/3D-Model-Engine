#include "ModelTexture.h"

ModelTexture::ModelTexture(const unsigned int id) :
m_texture_id(id) {}

void ModelTexture::set_shine_damper(const float shine_damper) {
	m_shine_damper = shine_damper;
}

float ModelTexture::get_shine_damper() const {
	return m_shine_damper;
}

void ModelTexture::set_reflectivity(const float reflectivity) {
	m_reflectivity = reflectivity;
}

float ModelTexture::get_reflectivity() const {
	return m_reflectivity;
}

unsigned int ModelTexture::get_id() const {
	return m_texture_id;
}

void ModelTexture::set_normal_map(const unsigned int value) {
	m_normal_map = value;
}

int ModelTexture::get_normal_map() const {
	return static_cast<int>(m_normal_map);
}

void ModelTexture::set_specular_map(const unsigned int value) { 
	m_specular_map = value;
	m_has_specular_map = true; 
}

bool ModelTexture::has_specular_map() const {
	return m_has_specular_map;
}

unsigned int ModelTexture::get_specular_map() const {
	return m_specular_map;
}

void ModelTexture::set_transparency(const bool value) {
	m_has_transparency = value;
}

bool ModelTexture::get_transparency() const {
	return m_has_transparency;
}

void ModelTexture::use_fake_lighting(const bool value) {
	m_use_fake_lighting = value;
}

bool ModelTexture::is_using_fake_lighting() const {
	return m_use_fake_lighting;
}

void ModelTexture::set_number_of_rows(const int value) {
	m_number_of_rows = value;
}

int ModelTexture::get_number_of_rows() const {
	return m_number_of_rows;
}