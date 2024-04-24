#include "HitTable.h"

#include "HitTable.h"

float Sphere::Hit(const Ray& ray, const HitRecord& record)  const
{
	glm::vec3 origin = ray.m_Origin - m_Center;

	float a = glm::dot(ray.m_Direction, ray.m_Direction);
	float b = 2.0f * (glm::dot(origin, ray.m_Direction));
	float c = glm::dot(origin, origin) - (m_Radius * m_Radius);

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
		return -1.0f;

	float tempT = (-b - sqrt(discriminant)) / (2.0f * a);
	return tempT;
}