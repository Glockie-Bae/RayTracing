#include "Renderer.h"
#include<iostream>

uint32_t Renderer::GetWidth()
{
	return m_FinalImage->GetWidth();
}

uint32_t Renderer::GetHeight()
{
	return m_FinalImage->GetHeight();
}






glm::vec3 Renderer::RayColor(const HitRecord& record)
{
	glm::vec3 lightDir = glm::normalize(m_LightDir);
	float lightIntensity = glm::max(glm::dot(record.normal, -lightDir), 0.0f);

	Sphere sphere = m_ActiveWorld.world[record.object_id];
	glm::vec3 sphereColor = sphere.m_Albedo;
	sphereColor *= lightIntensity;

	return sphereColor;
}


HitRecord Renderer::TraceRay(const World& world, Ray ray)
{
	// camrea position  -> ray.Origin
	// object position  -> ray.Direction



	int closestSphereID = -1;
    float closestT = std::numeric_limits<float>::max();

	for (int i = 0;i < m_ActiveWorld.Size(); i++)
	{
		const Sphere& sphere = m_ActiveWorld.world[i];

		HitRecord record;
		float tempT = sphere.Hit(ray, record);

		
		if (tempT > 0.0f && tempT < closestT) {
			closestSphereID = i;
			closestT = tempT;
		}
	}

	if (closestSphereID < 0)
		return Miss(ray);



	return ClosestHit(ray, closestSphereID, closestT);

}

HitRecord Renderer::ClosestHit(const Ray& ray, int closestSphereID, float closestT)
{
	HitRecord record;
	Sphere sphere = m_ActiveWorld.world[closestSphereID];

	record.t = closestT;
	record.hit_p = ray.m_Origin + closestT * ray.m_Direction;
	record.normal = glm::normalize(record.hit_p - sphere.GetCenter());
	record.object_id = closestSphereID;

	return record;

}

HitRecord Renderer::Miss(const Ray& ray)
{
	HitRecord record;
	record.t = -1;
	return record;
}

void Renderer::Render(const World& world, const Camera& camera)
{
	m_ActiveWorld = world;

	float aspectRatio = (float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
;
			glm::vec4 color = PerPixel(camera, x, y);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::PerPixel(const Camera& camera, uint32_t x, uint32_t y)
{
	Ray ray;
	ray.m_Origin = camera.GetPosition();
	ray.m_Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	glm::vec3 finalColor(0.0f);
	float multiplier = 1.0f;
	int bounces = 2;
	for (int i = 0; i < bounces; i++) {
		HitRecord record = TraceRay(m_ActiveWorld, ray);
		if (record.t < 0) {
			glm::vec3 skyColor = glm::vec3(0.0f, 0.0f, 0.0f);
			finalColor += skyColor * multiplier;
			break;
		}

		finalColor += RayColor(record) * multiplier;
		multiplier *= 0.7f;

		ray.m_Origin = record.hit_p + 0.001f * record.normal;
		ray.m_Direction = glm::reflect(ray.m_Direction, record.normal);
	}


	return glm::vec4(finalColor,1.0f);
}



uint32_t Renderer::ConvertToRGBA(const glm::vec4& color)
{
	uint8_t r = uint8_t(color.r * 255.0f);
	uint8_t g = uint8_t(color.g * 255.0f);
	uint8_t b = uint8_t(color.b * 255.0f);
	uint8_t a = uint8_t(color.a * 255.0f);

	uint32_t rgba = (a << 24) | (b << 16) | (g << 8) | (r << 0);
	return rgba;
}


void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage) {

		if (m_FinalImage->GetHeight() == height && m_FinalImage->GetWidth() == width)
			return;

		m_FinalImage->Resize(width, height);

	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);

	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}