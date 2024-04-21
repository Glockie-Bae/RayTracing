#pragma once


#include"Ray.h"
#include<memory>

struct HitRecord {
	float t;
	glm::vec3 hit_p;
	glm::vec3 normal;

	bool front_face;

};

class HitTable {
public:
	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitRecord& record, glm::vec3 light) const = 0;
};

class Sphere : public HitTable {
public:
	Sphere() {
		m_Center = glm::vec3(0.0f);
		m_Radius = 0.5f;
		m_Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	Sphere(const glm::vec3& center, float radius, const glm::vec3& albedo)
		: m_Center(center), m_Radius(radius), m_Albedo(albedo){};
	bool Hit(const Ray& ray,float t_min, float t_max, HitRecord& record, glm::vec3 light) const;

	glm::vec3 GetAlbedo() const { return m_Albedo; }
	glm::vec3 GetCenter() const { return m_Center; }
	float GetRadius() const { return m_Radius; }


	glm::vec3 m_Center;
	float m_Radius;
	glm::vec3 m_Albedo;
};



