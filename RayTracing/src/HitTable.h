#pragma once


#include"Ray.h"
#include<memory>

struct HitRecord {
	float t;
	glm::vec3 hit_p;
	glm::vec3 normal;

	int object_id;

};

class HitTable {
public:
	virtual float Hit(const Ray& ray, const HitRecord& record) const =0;
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
	float Hit(const Ray& ray, const HitRecord& record) const;

	glm::vec3 GetAlbedo() const { return m_Albedo; }
	glm::vec3 GetCenter() const { return m_Center; }
	float GetRadius() const { return m_Radius; }


	glm::vec3 m_Center;
	float m_Radius;
	glm::vec3 m_Albedo;
};



