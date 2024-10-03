#pragma once

#include <Boksi.h>
#include "imgui/imgui/imgui.h"
#include <chrono>

#include "Boksi/World/Mesh/VoxelMeshModifier.h"

const std::string res_path = "../Boksi/res/";

imgui_addons::ImGuiFileBrowser file_dialog; // As a class member or globally

constexpr int WORLD_SIZE = 512;
constexpr glm::uvec3 WORLD_DIMENSIONS = {WORLD_SIZE, WORLD_SIZE, WORLD_SIZE};

constexpr float VOXEL_SIZE = .5f;

const bool UPDATE_MODEL = false;

const Boksi::WindowProps WINDOW_PROPS = {
	"Voxel Ray Tracer",
	1280,
	720};

class ExampleLayer : public Boksi::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),
		  m_CameraController(45.0f, 0.1f, 100.0f),
		  m_EntitiesArray(std::make_shared<Boksi::EntitiesArray>())
	{
		BK_PROFILE_FUNCTION();
		AttachShadersAndBuffers();

		// Add entities

		std::shared_ptr<Boksi::Object> object = std::make_shared<Boksi::Object>(glm::vec3(100, 30, 100), glm::vec3(0, 0, 0), 1, "Donut");
		object->CreateModel(Boksi::ModelLoader::LoadModelToEntity(res_path + "Models/donut.txt", 1));

		m_EntitiesArray->AddEntity(object);

		// Set Camera
		m_CameraController.GetCamera().OnResize(1280, 720);
		m_CameraController.SetCameraMoveSpeed(0.05f);
		m_CameraController.SetCameraMouseSensitivity(0.0003f);
		m_CameraController.OnUpdate(0.0f);

		// Set the clear color
		Boksi::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});

		// Set the camera position and direction
		m_CameraController.GetCamera().SetPosition({20.0f, 29.0f, 24.0f});
		m_CameraController.GetCamera().SetForwardDirection({0.1, -0.5, -0.8});

		Boksi::ModelLoader::LoadModel(res_path + "Models/medieval_fantasy_book_70x30x50.txt", m_VoxelMesh, {0, 30, 0});

		std::shared_ptr<Boksi::ComputeShader> m_ComputeShader = m_VoxelRendererArray->GetComputeShader();
		m_ComputeShader->Bind();
		m_ComputeShader->UniformUploader->UploadUniformFloat3("u_LightPosition", glm::vec3(15.0f, 240.0f, 10.0f));
	}

	void OnUpdate(Boksi::TimeStep ts) override
	{
		BK_PROFILE_FUNCTION();

		m_CumulativeTime += ts;

		Boksi::RenderCommand::Clear();

		// if mouse click

		if (drawing)
		{
			glm::vec3 position = m_CameraController.GetCamera().GetPosition() / VOXEL_SIZE;

			glm::vec3 dimension = {10, 10, 10};
			if (Boksi::Input::IsKeyPressed(Boksi::Key::F))

			{
				Boksi::ModelLoader::LoadModel(loadedPath, m_VoxelMesh, position, m_CameraController.GetCamera().GetForwardDirection(), 1);
				m_VoxelMesh->MeshChanged = true;
			}
			else if (Boksi::Input::IsKeyPressed(Boksi::Key::R))
			{
				Boksi::ModelLoader::LoadModel(loadedPath, m_VoxelMesh, position,  m_CameraController.GetCamera().GetForwardDirection(),1, true);
				m_VoxelMesh->MeshChanged = true;
			}
		}

		// Render
		Boksi::Renderer::BeginScene();

		if (UPDATE_MODEL)
		{
			m_EntitiesArray->OnUpdate();
			Boksi::VoxelModifier::Draw(m_VoxelMesh, *m_EntitiesArray);
		}

		// Array Renderer

		// Material
		m_MaterialStorageBuffer->Bind(2);
		if (Boksi::MaterialLibrary::s_NewMaterialAdded)
		{
			m_MaterialStorageBuffer->SetData(Boksi::MaterialLibrary::GetMaterialData(), Boksi::MaterialLibrary::GetMaterialCount() * sizeof(Boksi::Material));
			Boksi::MaterialLibrary::s_NewMaterialAdded = false;
		}
		// Array Renderer
		// m_VoxelRendererArray->Render(m_CameraController.GetCamera(), m_Texture, m_VoxelMesh, VOXEL_SIZE, {1280, 720}, {16, 16, 1});
		m_VoxelRendererSvo->GetComputeShader()->Bind();
		m_VoxelRendererSvo->GetComputeShader()->UniformUploader->UploadUniformFloat("u_Time", m_CumulativeTime.GetSeconds() / 5);
		m_VoxelRendererSvo->Render(m_CameraController.GetCamera(), m_Texture, VOXEL_SIZE, m_VoxelMesh, {1280, 720}, {16, 16, 1});

		// Check for errors
		Boksi::RenderCommand::CheckForErrors();

		// Render the texture to the screen
		m_Texture->Bind(0);
		m_TextureShader->Bind();
		m_TextureShader->UniformUploader->UploadUniformInt("screenTexture", 0);
		Boksi::Renderer::Submit(m_TextureShader, m_TextureVertexArray);

		m_CameraController.OnUpdate(ts);
		Boksi::Renderer::EndScene();
	}

	virtual void OnImGuiRender(Boksi::TimeStep ts) override
	{
		BK_PROFILE_FUNCTION();

		ImGui::Begin("Data");
		auto &camera = m_CameraController.GetCamera();
		ImGui::Text("Camera Position: (%f, %f, %f)", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		ImGui::Text("Camera Direction: (%f, %f, %f)", camera.GetForwardDirection().x, camera.GetForwardDirection().y, camera.GetForwardDirection().z);
		ImGui::Text("Camera Up: (%f, %f, %f)", camera.GetUpDirection().x, camera.GetUpDirection().y, camera.GetUpDirection().z);
		ImGui::Text("Camera FOV: %f", camera.GetVerticalFOV());
		ImGui::Text("Last frame time: %f ms", ts.GetMilliseconds());
		ImGui::Text("FPS: %f", 1.0f / ts.GetSeconds());
		ImGui::End();

		ImGui::Begin("World");

		ImGui::SetWindowSize(ImVec2(300, 300));

		if (ImGui::Button("Recenter Camera"))
		{
			m_CameraController.GetCamera().SetPosition({10, 5, -10});
			glm::vec3 forward = glm::normalize(glm::vec3(glm::vec3(0, 0, 0)) - m_CameraController.GetCamera().GetPosition());
			m_CameraController.GetCamera().SetForwardDirection(forward);
			;
		}

		if (ImGui::Button("Add World Floor"))
		{
			Boksi::VoxelModifier::DrawFloor(m_VoxelMesh, Boksi::MaterialLibrary::GetMaterialID("Green"));
			m_VoxelMesh->MeshChanged = true;
		}

		ImGui::End();

		// Lights
		std::shared_ptr<Boksi::ComputeShader> m_ComputeShader = m_VoxelRendererArray->GetComputeShader();
		ImGui::Begin("Lights");

		static float shadowBias = 0.0f;
		ImGui::SliderFloat("Shadow Bias", &shadowBias, 0.0f, 1.0f);
		m_ComputeShader->Bind();
		m_ComputeShader->UniformUploader->UploadUniformFloat("u_ShadowBias", shadowBias);

		static float shadowStrength = 0.5f;
		ImGui::SliderFloat("Shadow Strength", &shadowStrength, 0.0f, 1.0f);
		m_ComputeShader->Bind();
		m_ComputeShader->UniformUploader->UploadUniformFloat("u_ShadowStrength", shadowStrength);

		// tick mark for Camera control shadow
		static bool cameraControlShadow = false;
		ImGui::Checkbox("Camera Control Shadow", &cameraControlShadow);
		if (cameraControlShadow)
		{
			m_ComputeShader->Bind();
			m_ComputeShader->UniformUploader->UploadUniformFloat3("u_LightPosition", m_CameraController.GetCamera().GetPosition());
		}

		bool open = false, save = false;

		if (ImGui::Button("Open File Dialog"))
			open = true;
		ImGui::SameLine();
		if (ImGui::Button("Save File Dialog"))
			save = true;

		// Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
		if (open)
			ImGui::OpenPopup("Open File");
		if (save)
			ImGui::OpenPopup("Save File");

		if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".txt"))
		{
			loadedPath = file_dialog.selected_path;
		}

		if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".txt"))
		{
			std::cout << file_dialog.selected_fn << std::endl;	 // The name of the selected file or directory in case of Select Directory dialog mode
			std::cout << file_dialog.selected_path << std::endl; // The absolute path to the selected file
			std::cout << file_dialog.ext << std::endl;			 // Access ext separately (For SAVE mode)

			Boksi::ModelLoader::SaveMeshToFile(file_dialog.selected_path, m_VoxelMesh);
		}

		if (loadedPath != "")
		{

			ImGui::Text("Loaded Path: %s", loadedPath.c_str());

			if (ImGui::Button("X"))
			{
				loadedPath = "";
			}

			if (ImGui::Button("Load"))
			{
				// get voxel pos
				glm::vec3 position = m_CameraController.GetCamera().GetPosition();
				position = position * VOXEL_SIZE;
				Boksi::ModelLoader::LoadModel(loadedPath, m_VoxelMesh, position, m_CameraController.GetCamera().GetForwardDirection(), 1);
				m_VoxelMesh->MeshChanged = true;
			}

			if (ImGui::Button("Draw"))
			{
				if (drawing)
					drawing = false;
				else
					drawing = true;
			}
		}

		ImGui::End();
	}

	void OnEvent(Boksi::Event &event) override
	{
	}

private:
	std::shared_ptr<Boksi::Shader> m_TextureShader;
	std::shared_ptr<Boksi::VertexArray> m_TextureVertexArray;
	std::shared_ptr<Boksi::Texture2D> m_Texture;

	Boksi::CameraController m_CameraController;

	std::shared_ptr<Boksi::VoxelRendererArray> m_VoxelRendererArray;
	// std::shared_ptr<Boksi::VoxelMeshArray> m_VoxelMesh;

	std::shared_ptr<Boksi::VoxelMeshSVO> m_VoxelMesh;
	std::shared_ptr<Boksi::VoxelRendererSVO> m_VoxelRendererSvo;

	std::shared_ptr<Boksi::EntitiesArray> m_EntitiesArray;
	std::shared_ptr<Boksi::StorageBuffer> m_MaterialStorageBuffer;
	Boksi::TimeStep m_CumulativeTime;
	bool drawing = false;

	std::string loadedPath = res_path + "Models/donut.txt";

	void AttachShadersAndBuffers();
};

class Sandbox : public Boksi::Application
{
public:
	Sandbox()
	{
		BK_PROFILE_FUNCTION();
		PushLayer(new ExampleLayer());
	}
	~Sandbox() override = default;
};

//Boksi::Application *Boksi::CreateApplication()
//{
//	return new Sandbox();
//}

void ExampleLayer::AttachShadersAndBuffers()
{
	BK_PROFILE_FUNCTION();
	/*
	// Define some materials
	Boksi::MaterialLibrary::AddMaterial({
											glm::vec3(1.0f, 0.0f, 0.0f), // Albedo (base color)
											glm::vec3(0.1f, 0.1f, 0.1f), // Specular color
											glm::vec3(0.2f, 0.0f, 0.0f)	 // Ambient color (dimmed red)
										},
										"Red");

	Boksi::MaterialLibrary::AddMaterial({
											glm::vec3(0.0f, 0.3f, 0.0f), // Albedo (base color)
											glm::vec3(0.1f, 0.1f, 0.1f), // Specular color
											glm::vec3(0.0f, 0.2f, 0.0f)	 // Ambient color (dimmed green)
										},
										"Green");

	Boksi::MaterialLibrary::AddMaterial({
											glm::vec3(0.0f, 0.0f, 1.0f), // Albedo (base color)
											glm::vec3(0.1f, 0.1f, 0.1f), // Specular color
											glm::vec3(0.0f, 0.0f, 0.2f)	 // Ambient color (dimmed blue)
										},
										"Blue");

	Boksi::MaterialLibrary::AddMaterial({
											glm::vec3(1.0f, 1.0f, 0.0f), // Albedo (base color)
											glm::vec3(0.1f, 0.1f, 0.1f), // Specular color
											glm::vec3(0.2f, 0.2f, 0.0f)	 // Ambient color (dimmed yellow)
										},
										"Yellow");
										*/

	// Material Storage Buffer
	m_MaterialStorageBuffer.reset(Boksi::StorageBuffer::Create());

	// Compute Shader
	m_VoxelRendererArray.reset(new Boksi::VoxelRendererArray(res_path + "Shaders/ray_trace.comp.glsl"));
	m_VoxelRendererSvo.reset(new Boksi::VoxelRendererSVO(res_path + "Shaders/ray_trace_svo_better.comp.glsl"));
	m_VoxelMesh.reset(new Boksi::VoxelMeshSVO(WORLD_DIMENSIONS));

	// Normal frag and vertex shader
	const std::string vertex_src = Boksi::Renderer::ReadFile(res_path + "Shaders/texture.vertex.glsl");
	const std::string fragment_src = Boksi::Renderer::ReadFile(res_path + "Shaders/texture.fragment.glsl");

	m_TextureShader.reset(Boksi::Shader::Create(vertex_src, fragment_src));

	// Vertex Array
	m_TextureVertexArray.reset(Boksi::VertexArray::Create());
	// Full screen quad
	float quad_vertices[] = {
		// positions        // texture Coords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0};

	Boksi::BufferLayout layout = {
		{Boksi::ShaderDataType::Float2, "a_Position"},
		{Boksi::ShaderDataType::Float2, "a_TexCoord"}};
	std::shared_ptr<Boksi::VertexBuffer> vertex_buffer;
	vertex_buffer.reset(Boksi::VertexBuffer::Create(quad_vertices, sizeof(quad_vertices)));
	vertex_buffer->SetLayout(layout);
	m_TextureVertexArray->AddVertexBuffer(vertex_buffer);

	// Index Buffer
	std::shared_ptr<Boksi::IndexBuffer> index_buffer;
	index_buffer.reset(Boksi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_TextureVertexArray->SetIndexBuffer(index_buffer);

	// Texture
	Boksi::TextureSpecification spec;
	spec.Width = 1280;
	spec.Height = 720;

	spec.Format = Boksi::ImageFormat::RGBA8;
	m_Texture = Boksi::Texture2D::Create(spec);
}
