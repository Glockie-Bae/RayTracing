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






glm::vec3 Renderer::RayColor(Ray ray, glm::vec3 light, Sphere sphere)
{
	glm::vec3 origin = ray.m_Origin - sphere.m_Center;
	glm::vec3 hitPoint = origin + ray.m_Direction * ray.m_T;
	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDir = glm::normalize(m_LightDir);
	float lightIntensity = glm::max(glm::dot(normal, -lightDir), 0.0f);

	glm::vec3 sphereColor = sphere.m_Albedo;
	sphereColor *= lightIntensity;

	return sphereColor;
}


glm::vec4 Renderer::TraceRay(const World& world, Ray ray)
{
	// camrea position  -> ray.Origin
	// object position  -> ray.Direction


	if (world.world.size() == 0)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	const Sphere* closestSphere = nullptr;
	ray.m_T = std::numeric_limits<float>::max();
	for (const Sphere& sphere : world.world)
	{
		glm::vec3 origin = ray.m_Origin - sphere.GetCenter();

		float a = glm::dot(ray.m_Direction, ray.m_Direction);
		float b = 2.0f * (glm::dot(origin, ray.m_Direction));
		float c = glm::dot(origin, origin) - (sphere.m_Radius * sphere.m_Radius);

		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f)
			continue;

		float tempT = (-b - sqrt(discriminant)) / (2.0f * a);
		if (tempT < ray.m_T) {
			closestSphere = &sphere;
			ray.m_T = tempT;
		}
	}

	if (closestSphere == nullptr)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);




	return glm::vec4(RayColor(ray, m_LightDir, *closestSphere), 1.0f);

}

void Renderer::Render(const World& world, const Camera& camera)
{
	Ray ray;
	ray.m_Origin = camera.GetPosition();

	float aspectRatio = (float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			glm::vec2 coord( (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight());

			coord = coord * 2.0f - 1.0f;
			coord.x *= aspectRatio;

			ray.m_Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

			glm::vec4 color = TraceRay(world, ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageData);
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
