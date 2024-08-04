#include <Boksi.h>
#include "imgui/imgui/imgui.h"
#include <chrono>

#include "Boksi/World/Mesh/VoxelMeshModifier.h"

const std::string res_path = "Boksi/res/";

constexpr int WORLD_SIZE = 256;
constexpr glm::uvec3 WORLD_DIMENSIONS = {WORLD_SIZE, WORLD_SIZE, WORLD_SIZE};

constexpr float VOXEL_SIZE = .5f;

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
		AttachShadersAndBuffers();

		// Add entities
		// std::shared_ptr<Boksi::Entity> entity = std::make_shared<Boksi::Cube>(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));
		m_EntitiesArray->AddEntity(std::make_shared<Boksi::Cube>(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 4));

		// Set Camera
		m_CameraController.GetCamera().OnResize(1280, 720);
		m_CameraController.SetCameraMoveSpeed(1.0f);
		m_CameraController.SetCameraMouseSensitivity(0.01f);
		m_CameraController.OnUpdate(0.0f);

		// Set the clear color
		Boksi::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});

		// Set the camera position and direction
		m_CameraController.GetCamera().SetPosition({-1, -1, -1});
		m_CameraController.GetCamera().SetForwardDirection({1, 1, 1});

		// Load Models
		Boksi::ModelLoader::LoadModel(res_path + "Models/donut.txt", m_VoxelMesh, {0, 0, 0}, 1);
		Boksi::ModelLoader::LoadModel(res_path + "Models/llama04.txt", m_VoxelMesh, {0, 30, 0}, 1);
	}

	void OnUpdate() override
	{
		// Time calculation
		auto now = std::chrono::high_resolution_clock::now();
		m_LastFrameTime = std::chrono::duration<float, std::milli>(now - m_Time).count();
		m_Time = now;

		Boksi::RenderCommand::Clear();

		// Render
		Boksi::Renderer::BeginScene();

		m_EntitiesArray->OnUpdate();
		Boksi::VoxelModifier::Draw(m_VoxelMesh, *m_EntitiesArray);

		// Array Renderer

		// Material
		m_MaterialStorageBuffer->Bind(1);
		if (Boksi::MaterialLibrary::s_NewMaterialAdded)
		{
			m_MaterialStorageBuffer->SetData(Boksi::MaterialLibrary::GetMaterialData(), Boksi::MaterialLibrary::GetMaterialCount() * sizeof(Boksi::Material));
			Boksi::MaterialLibrary::s_NewMaterialAdded = false;
		}
		// Array Renderer
		m_VoxelRendererArray->Render(m_CameraController.GetCamera(), m_Texture, m_VoxelMesh, VOXEL_SIZE, {1280, 720}, {16, 16, 1});

		// Check for errors
		Boksi::RenderCommand::CheckForErrors();

		// Render the texture to the screen
		m_Texture->Bind(0);
		m_TextureShader->Bind();
		m_TextureShader->UniformUploader->UploadUniformInt("screenTexture", 0);
		Boksi::Renderer::Submit(m_TextureShader, m_TextureVertexArray);

		m_CameraController.OnUpdate(1);
		Boksi::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Data");
		auto &camera = m_CameraController.GetCamera();
		ImGui::Text("Camera Position: (%f, %f, %f)", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		ImGui::Text("Camera Direction: (%f, %f, %f)", camera.GetForwardDirection().x, camera.GetForwardDirection().y, camera.GetForwardDirection().z);
		ImGui::Text("Camera Up: (%f, %f, %f)", camera.GetUpDirection().x, camera.GetUpDirection().y, camera.GetUpDirection().z);
		ImGui::Text("Camera FOV: %f", camera.GetVerticalFOV());
		ImGui::Text("Last frame time: %f", m_LastFrameTime);
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
			Boksi::VoxelModifier::DrawFloor(m_VoxelMesh, Boksi::MaterialLibrary::GetMaterialID("Green") );
		}

		ImGui::End();

		// Lights
		std::shared_ptr<Boksi::ComputeShader> m_ComputeShader = m_VoxelRendererArray->GetComputeShader();
		ImGui::Begin("Lights");


		static float shadowBias = 0.01f;
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
	std::shared_ptr<Boksi::VoxelMeshArray> m_VoxelMesh;

	std::shared_ptr<Boksi::EntitiesArray> m_EntitiesArray;
	std::shared_ptr<Boksi::StorageBuffer> m_MaterialStorageBuffer;

	std::chrono::time_point<std::chrono::high_resolution_clock> m_Time = std::chrono::high_resolution_clock::now();
	glm::vec3 m_LightPosition = {0, 0, 0};
	float m_LastFrameTime = 0.0f;

public:
	void AttachShadersAndBuffers();
};

class Sandbox : public Boksi::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() override = default;
};

Boksi::Application *Boksi::CreateApplication()
{
	return new Sandbox();
}

void ExampleLayer::AttachShadersAndBuffers()
{
	// Define some materials
	Boksi::MaterialLibrary::AddMaterial({
											glm::vec3(1.0f, 0.0f, 0.0f), // Albedo (base color)
											glm::vec3(0.1f, 0.1f, 0.1f), // Specular color
											glm::vec3(0.2f, 0.0f, 0.0f)	 // Ambient color (dimmed red)
										},
										"Red");

	Boksi::MaterialLibrary::AddMaterial({
											glm::vec3(0.0f, 1.0f, 0.0f), // Albedo (base color)
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

	// Material Storage Buffer
	m_MaterialStorageBuffer.reset(Boksi::StorageBuffer::Create());

	// Compute Shader
	m_VoxelRendererArray.reset(new Boksi::VoxelRendererArray(res_path + "Shaders/ray_trace.comp.glsl"));
	m_VoxelMesh.reset(new Boksi::VoxelMeshArray(WORLD_DIMENSIONS));

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
