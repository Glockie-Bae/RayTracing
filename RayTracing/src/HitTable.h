#pragma once


#include"Ray.h"
#include<memory>

struct HitRecord {
	float t;
	glm::vec3 hit_p;
	glm::vec3 normal;

	int object_id;

};

struct Material {
	glm::vec3 albedo{1.0f};
	float roughness = 1.0f;
	float metallic = 0.0f;

	Material() {};
	Material(const glm::vec3& abd, float rough, float metal)
		:albedo(abd), roughness(rough), metallic(metal){}
};

class HitTable {
public:
	virtual float Hit(const Ray& ray, const HitRecord& record) const =0;
	virtual void SetMaterialID(int id) = 0;

	int m_MaterialID = 0;
};

class Sphere : public HitTable {
public:

	Sphere() {
		m_Center = glm::vec3(0.0f);
		m_Radius = 0.5f;
	}
	
	Sphere(const glm::vec3& center, float radius)
		: m_Center(center), m_Radius(radius)
	{
	};
	float Hit(const Ray& ray, const HitRecord& record) const;
	void SetMaterialID(int id) { m_MaterialID = id; }
public:
	glm::vec3 m_Center;
	float m_Radius;
};



