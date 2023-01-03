#pragma once

class ModelTexture {
public:
	ModelTexture(unsigned int id);

	void set_shine_damper(float shine_damper);
	float get_shine_damper() const;

	void set_reflectivity(float reflectivity);
	float get_reflectivity() const;

	unsigned int get_id() const;

	void set_normal_map(unsigned int value);
	int get_normal_map() const;

	void set_specular_map(unsigned int value);

	bool has_specular_map() const;
	unsigned int get_specular_map() const;

	void set_transparency(bool value);
	bool get_transparency() const;

	void use_fake_lighting(bool value);
	bool is_using_fake_lighting() const;

	void set_number_of_rows(int value);
	int get_number_of_rows() const;

private:
	unsigned int m_texture_id,
				 m_normal_map,
				 m_specular_map;

	float m_shine_damper = .5f;
	float m_reflectivity = 0.0f;

	bool m_has_transparency = false,
		 m_use_fake_lighting = false,
		 m_has_specular_map = false;

	int m_number_of_rows = 1;
};