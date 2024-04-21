#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include"Walnut\Random.h"
#include"Walnut\Timer.h"

#include <glm/gtc/type_ptr.hpp>

#include"Renderer.h"
#include"Camrea.h"


using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.1f, 100.0f)
	{
		{
			glm::vec3 sphere_pos(1.5f, 0.0f, -3.0f);
			float sphere_radius = 1.0f;
			glm::vec3 sphere_albedo(0.2f, 0.3f, 1.0f);
			Sphere sphere(sphere_pos, sphere_radius, sphere_albedo);
			m_World.Add(sphere);
		}

		{
			glm::vec3 sphere_pos(-0.5f, 0.0f, -6.0f);
			float sphere_radius = 1.5f;
			glm::vec3 sphere_albedo(1.0f, 1.0f, 1.0f);
			Sphere sphere(sphere_pos, sphere_radius, sphere_albedo);
			m_World.Add(sphere);
		}





	}

	virtual void OnUpdate(float ts) override {
		m_Camera.OnUpdate(ts);

	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Setting");
		ImGui::Text("Last render %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render")) {
			Render();
		}


		ImGui::DragFloat3("Light", glm::value_ptr(m_Renderer.m_LightDir), 0.1f);

		ImGui::Text("m_World size %i", m_World.Size());

		ImGui::End();

		ImGui::Begin("World");
		for (int i = 0; i < m_World.Size(); i++) {
			ImGui::PushID(i);

			ImGui::Text("Sphere %i: ", i + 1);
			ImGui::DragFloat3("Position", glm::value_ptr(m_World.world[i].m_Center), 0.1f);
			ImGui::DragFloat("Radius", &m_World.world[i].m_Radius, 0.1f);
			ImGui::ColorEdit3("Albedo", glm::value_ptr(m_World.world[i].m_Albedo), 0.1f);

			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("ViewPort");

		m_ViewPortHeight = ImGui::GetContentRegionAvail().y;
		m_ViewPortWidth = ImGui::GetContentRegionAvail().x;

		std::shared_ptr<Walnut::Image> image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)m_Renderer.GetWidth(), (float)m_Renderer.GetHeight() }, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render() {
		Timer timer;
		m_Renderer.OnResize(m_ViewPortWidth, m_ViewPortHeight);
		m_Camera.OnResize(m_ViewPortWidth, m_ViewPortHeight);
		m_Renderer.Render(m_World, m_Camera);


		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer m_Renderer;
	Camera m_Camera;
	World m_World;
	uint32_t m_ViewPortWidth = 0, m_ViewPortHeight = 0;
	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				ImGui::EndMenu();
			}
		});
	return app;
}