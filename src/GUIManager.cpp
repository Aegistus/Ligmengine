#include <GUIManager.h>
#include <Engine.h>
#include <imgui.h>
#include <backends/imgui_impl_wgpu.h>
#include <backends/imgui_impl_glfw.h>

namespace Ligmengine
{
	void GUIManager::Startup()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::GetIO();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOther(gEngine.graphics.window, true);
		ImGui_ImplWGPU_Init(gEngine.graphics.device, 3, gEngine.graphics.swap_chain_format);
	}

	void GUIManager::Draw(WGPURenderPassEncoder renderPass)
	{
		// Start the Dear ImGui frame
		ImGui_ImplWGPU_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Build our UI
		DrawHierarchyWindow();
		if (showComponentWindow)
		{
			DrawComponentWindow(currentlyInspecting);
		}

		// Draw the UI
		ImGui::EndFrame();
		// Convert the UI defined above into low-level drawing commands
		ImGui::Render();
		// Execute the low-level drawing commands on the WebGPU backend
		ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), renderPass);
	}

	void GUIManager::Shutdown()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplWGPU_Shutdown();
	}

	void GUIManager::DrawHierarchyWindow()
	{
		static bool show_demo_window = true;
		static bool show_another_window = false;
		static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		ImGui::Begin("Entities");                                // Create a window called "Hello, world!" and append into it.

		ImGui::Text("These are the entities in the scene:");                     // Display some text (you can use a format strings too)

		gEngine.ecs.ForEach<EntityData>
			([&] (EntityID e)
				{
					EntityData data = gEngine.ecs.GetComponent<EntityData>(e);
					if (ImGui::Button(data.name.c_str()))
					{
						showComponentWindow = true;
						currentlyInspecting = e;
					}
				}
			);

		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	void GUIManager::DrawComponentWindow(EntityID e)
	{
		static bool show_demo_window = true;
		static bool show_another_window = false;
		static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		ImGui::Begin("Components");

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Attached Components");
		ImGui::BeginChild("Scrolling");
		if (gEngine.ecs.HasComponent<EntityData>(e))
		{
			if (ImGui::CollapsingHeader("Entity Data"))
			{
				ImGui::Text("Entity name: %s", gEngine.ecs.GetComponent<EntityData>(e).name);
			}
		}
		if (gEngine.ecs.HasComponent<Transform>(e))
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				Transform& t = gEngine.ecs.GetComponent<Transform>(e);
				ImGui::Text("Position: %.2f %.2f %.2f", t.position.x, t.position.y, t.position.z);
				ImGui::Text("Rotation: %.2f", t.rotation);
				ImGui::Text("Scale: %.2f %.2f", t.scale.x, t.scale.y);
			}
		}
		if (gEngine.ecs.HasComponent<SpriteRenderer>(e))
		{
			if (ImGui::CollapsingHeader("Sprite Renderer"))
			{
				SpriteRenderer s = gEngine.ecs.GetComponent<SpriteRenderer>(e);
				ImGui::Text("Width: %.2f", s.sprite->width);
				ImGui::Text("Height: %.2f", s.sprite->height);
				ImGui::Text("Sprite: %s", s.sprite->name);
			}
		}
		if (gEngine.ecs.HasComponent<Script>(e))
		{
			if (ImGui::CollapsingHeader("Script"))
			{
				Script s = gEngine.ecs.GetComponent<Script>(e);
				ImGui::Text("Name: %s", s.name);
			}
		}
		if (gEngine.ecs.HasComponent<RigidBody>(e))
		{
			if (ImGui::CollapsingHeader("Rigid Body"))
			{
				RigidBody rb = gEngine.ecs.GetComponent<RigidBody>(e);
				ImGui::Text("Velocity: %.2f %.2f", rb.velocity);
			}
		}
		ImGui::EndChild();

		ImGui::End();
	}
}