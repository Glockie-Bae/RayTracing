#pragma once

#include"Walnut\Image.h"
#include"Walnut\Random.h"
#include<glm\glm.hpp>

#include"Camrea.h"
#include"HitTable.h"
#include"HitTableList.h"
#include<memory>
class Renderer {
public:

	Renderer() = default;
	void OnResize(uint32_t width, uint32_t height);
	
	void Render(const World& world, const Camera& camera);

	uint32_t ConvertToRGBA(const glm::vec4& color);

	uint32_t GetWidth();
	uint32_t GetHeight();

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

	void SetlightDir(const glm::vec3& lightDir) { m_LightDir = glm::normalize(lightDir); }

	glm::vec3 GetLightDir() const { return m_LightDir; }

	glm::vec3 m_LightDir = glm::vec3(-1,-1,-1);

	glm::vec3 RayColor(Ray ray, glm::vec3 light, Sphere sphere);
	
private:
	glm::vec4 TraceRay(const World& world, Ray ray);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};