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
		  m_EntitiesArray(new Boksi::EntitiesArray()),
		  m_World(new Boksi::World(WORLD_DIMENSIONS))
	{
		AttachShadersAndBuffers();

		// Draw a circle of voxels
		Boksi::VoxelModifier::DrawCircle(20, m_VoxelMesh->GetSize(), {40, 40, 40}, m_VoxelMesh, 1);

		// Set all voxels

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

		// Send Dimensions as Unfiorm
		m_ComputeShader->Bind();
		m_ComputeShader->UniformUploader->UploadUniformInt3("u_Dimensions", {m_World->GetSize().x, m_World->GetSize().y, m_World->GetSize().z});

		m_World->DrawCircle(30, 10, 10, 5, 1);
		m_World->DrawCircle(50, 10, 10, 5, 1);
	}

	void OnUpdate() override
	{
		// Time calculation
		auto now = std::chrono::high_resolution_clock::now();
		m_LastFrameTime = std::chrono::duration<float, std::milli>(now - m_Time).count();
		m_Time = now;

		Boksi::RenderCommand::Clear();
		m_EntitiesArray->Draw(m_VoxelMesh);

		// Render
		Boksi::Renderer::BeginScene();

		// Compute Shader
		m_ComputeShader->Bind();
		// Set the SSBO
		m_StorageBuffer->SetData(m_World->GetVoxelsData(), m_World->GetVoxelCount() * sizeof(Boksi::Voxel));
		// Bind SSBO
		m_StorageBuffer->Bind(1);
		// Camera Uniforms
		Boksi::Camera::UploadCameraUniformToShader(m_ComputeShader->UniformUploader, m_CameraController.GetCamera());
		// Bind the texture
		m_Texture->BindWrite(0);
		// Dispatch Compute Shader
		Boksi::Renderer::DispatchCompute(m_ComputeShader, 1280 / 16, 720 / 16, 1);

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

		ImGui::End();

		// Lights
		ImGui::Begin("Lights");

		// Set Values for u_Intensity
		static float intensity = 1.0f;
		ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f);
		m_ComputeShader->Bind();
		m_ComputeShader->UniformUploader->UploadUniformFloat("u_Intensity", intensity);

		// Set Values for u_Exposure
		static float exposure = 1.0f;
		ImGui::SliderFloat("Exposure", &exposure, 0.0f, 1.0f);
		m_ComputeShader->Bind();
		m_ComputeShader->UniformUploader->UploadUniformFloat("u_Exposure", exposure);

		// Set Values for u_AO
		static float ambient = 0.1f;
		ImGui::SliderFloat("Ambient", &ambient, 0.0f, 1.0f); // Proper range for AO
		m_ComputeShader->Bind();
		m_ComputeShader->UniformUploader->UploadUniformFloat("u_AO", ambient);

		static float shadowBias = 0.01f;
		ImGui::SliderFloat("Shadow Bias", &shadowBias, 0.0f, 1.0f);
		m_ComputeShader->Bind();
		m_ComputeShader->UniformUploader->UploadUniformFloat("u_ShadowBias", shadowBias);

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

	std::shared_ptr<Boksi::ComputeShader> m_ComputeShader;
	std::shared_ptr<Boksi::StorageBuffer> m_StorageBuffer;
	std::shared_ptr<Boksi::World> m_World;

	std::shared_ptr<Boksi::EntitiesArray> m_EntitiesArray;

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
	// Compute Shader
	m_VoxelRendererArray.reset(new Boksi::VoxelRendererArray(res_path + "Shaders/ray_trace.comp.glsl"));
	m_VoxelMesh.reset(new Boksi::VoxelMeshArray(WORLD_DIMENSIONS));

	// Compute Shader
	const std::string compute_src = Boksi::ShaderLoader::Load(res_path + "Shaders/ray_trace.comp.glsl");
	m_ComputeShader.reset(Boksi::ComputeShader::Create(compute_src));

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

	m_ComputeShader->Bind();
	m_ComputeShader->UniformUploader->UploadUniformInt3("u_Dimensions", {m_World->GetSize().x, m_World->GetSize().y, m_World->GetSize().z});

	// Storage Buffer
	m_StorageBuffer.reset(Boksi::StorageBuffer::Create());
}
