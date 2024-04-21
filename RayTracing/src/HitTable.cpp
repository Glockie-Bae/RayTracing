#include "HitTable.h"

bool Sphere::Hit(const Ray& ray, float t_min, float t_max, HitRecord& record, glm::vec3 light) const
{
	float a = glm::dot(ray.m_Direction, ray.m_Direction);
    float b = 2 * ((glm::dot(ray.m_Origin, ray.m_Direction)) - (glm::dot(ray.m_Direction, m_Center)));
    float c = glm::dot((ray.m_Origin-m_Center), (ray.m_Origin - m_Center)) - (m_Radius * m_Radius);
    
    //glm::vec3 origin = ray.m_Origin - m_Center;
    
    
    
    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0 
	/*float a = glm::dot(ray.m_Direction, ray.m_Direction);
	float b = 2 * glm::dot(origin, ray.m_Direction);
	float c = glm::dot(origin, origin) - (m_Radius * m_Radius);*/

	float discriminant = b * b - 4.0f * a * c;
    if (discriminant > 0) {
        auto root = sqrt(discriminant);
        auto temp = (-b - root) / 2 * a;
        {
            record.t = temp;
            record.hit_p = ray.at(temp);
            record.normal = glm::normalize(record.hit_p - m_Center);
            
           
            return true;
        }
        //temp = (-b + root) / a;
        //if (temp < t_max && temp > t_min) {
        //    record.t = temp;
        //    record.hit_p = ray.at(record.t);
        //    record.normal = glm::normalize(record.hit_p - m_Center) * 0.5f + 0.5f;
        //    /*if (glm::dot(record.normal, light) > 0) {
        //        record.normal = -record.normal;
        //    }*/
        //    return true;
        //}
    }
	return false;
}
