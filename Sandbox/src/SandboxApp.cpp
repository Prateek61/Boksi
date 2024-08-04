#include <Boksi.h>
#include "imgui/imgui/imgui.h"
#include <chrono>

const std::string res_path = "Boksi/res/";

const int Power = 6;
const int WORLD_SIZE = 256;

const float VOXEL_SIZE = .1f;


class ExampleLayer : public Boksi::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),
		  m_CameraController(45.0f, 0.1f, 100.0f),
		  m_World(new Boksi::World({WORLD_SIZE, WORLD_SIZE, WORLD_SIZE})),
		  m_VoxelRendererSVO(new Boksi::VoxelRendererSVO(res_path + "Shaders/ray_trace_svo.glsl")),
		  m_VoxelMeshSVO(new Boksi::VoxelMeshSVO(m_World->GetVoxels(), m_World->GetSize())),
		  m_VoxelMeshArray(new Boksi::VoxelMeshArray(m_World->GetSize()))

	{
		
		m_VoxelMeshArray->SetVoxel({0, 0, 0}, uint8_t(1));


		// Set Camera
		m_CameraController.GetCamera().OnResize(1280, 720);
		m_CameraController.SetCameraMoveSpeed(1.0f);
		m_CameraController.SetCameraMouseSensitivity(0.01f);
		m_CameraController.OnUpdate(0.0f);

		AttachShadersAndBuffers();

		// Add materials
		Boksi::MaterialLibrary::AddMaterial(Boksi::Material({0.8f, 0.8f, 0.8f}), "White");
		Boksi::MaterialLibrary::AddMaterial(Boksi::Material({0.8f, 0.2f, 0.2f}), "Red");
		Boksi::MaterialLibrary::AddMaterial(Boksi::Material({0.8f, 0.2f, 0.2f}), "Red");

		// Set the clear color
		Boksi::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});

		
		m_CameraController.GetCamera().SetPosition({-1, -1, -1});
		m_CameraController.GetCamera().SetForwardDirection({1, 1, 1});
		
		
		m_VoxelMeshSVO.reset(new Boksi::VoxelMeshSVO(m_World->GetVoxels(), m_World->GetSize()));

		// Load a model
		Boksi::ModelLoader::LoadModel("Boksi/res/Models/donut.txt", m_World, {10, 10, 0}, 1);
		Boksi::ModelLoader::LoadModel("Boksi/res/Models/donut.txt", m_World, {10, 10, 40}, 1);
		Boksi::ModelLoader::LoadModel("Boksi/res/Models/donut.txt", m_World, {10, 10, 80}, 1);

		Boksi::ModelLoader::LoadModel("Boksi/res/Models/birch_tree.txt", m_World, {50, 10, 0}, 1);
	}

	void AttachShadersAndBuffers()
	{
		// Compute Shader

		// Normal frag and vertex shader
		const std::string vertex_src = Boksi::Renderer::ReadFile(res_path + "Shaders/texture.vertex.glsl");
		const std::string fragment_src = Boksi::Renderer::ReadFile(res_path + "Shaders/texture.fragment.glsl");

		m_Shader.reset(Boksi::Shader::Create(vertex_src, fragment_src));

		// Vertex Array
		m_VertexArray.reset(Boksi::VertexArray::Create());
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
		m_VertexArray->AddVertexBuffer(vertex_buffer);

		// Index Buffer
		std::shared_ptr<Boksi::IndexBuffer> index_buffer;
		index_buffer.reset(Boksi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(index_buffer);

		// Texture
		Boksi::TextureSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;

		spec.Format = Boksi::ImageFormat::RGBA8;
		m_Texture = Boksi::Texture2D::Create(spec);

		// Storage Buffer
		m_StorageBuffer.reset(Boksi::StorageBuffer::Create());

		// Material Storage Buffer
		auto material_colors = Boksi::MaterialLibrary::GetColors();
		uint32_t material_count = material_colors.size();
		m_MaterialStorageBuffer.reset(Boksi::StorageBuffer::Create());
		m_MaterialStorageBuffer->Bind(2);
		m_MaterialStorageBuffer->SetData(material_colors.data(), material_count * sizeof(glm::vec3));

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


		// Bind SSBO
		m_StorageBuffer->Bind(1);
		// Set the SSBO
		m_StorageBuffer->SetData(m_World->GetVoxelsData(), m_World->GetVoxelCount() * sizeof(Boksi::Voxel));
		// Bind the material SSBO
		m_MaterialStorageBuffer->Bind(2);
		auto material_colors = Boksi::MaterialLibrary::GetColors();
		m_MaterialStorageBuffer->SetData(material_colors.data(), material_colors.size() * sizeof(glm::vec3));

		// Check for errors
		Boksi::RenderCommand::CheckForErrors();

		// Render the texture to the screen
		m_Texture->Bind(0);
		m_Shader->Bind();
		m_Shader->UniformUploader->UploadUniformInt("screenTexture", 0);
		Boksi::Renderer::Submit(m_Shader, m_VertexArray);

		m_VoxelRendererSVO->Render(m_CameraController.GetCamera(), m_Texture, m_World->GetSize(), m_VoxelMeshSVO->GetMaximumDepth() , VOXEL_SIZE, m_VoxelMeshArray);

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

		ImGui::Text("World Size: (%d, %d, %d)", m_World->GetSize().x, m_World->GetSize().y, m_World->GetSize().z);
		// increase imgui window size
		ImGui::SetWindowSize(ImVec2(300, 300));

		const glm::uvec3 size = m_World->GetSize();
		static int x = size.x;
		static int y = size.y;
		static int z = size.z;

		ImGui::InputInt("X", &x);
		ImGui::InputInt("Y", &y);
		ImGui::InputInt("Z", &z);

		if (ImGui::Button("Resize"))
		{
			BK_CORE_TRACE("Resizing world to ({0}, {1}, {2})", x, y, z);
		}

		if (ImGui::Button("Clear Screen"))
		{
			m_World->ClearScreen(0);
		}

		if (ImGui::Button("Randomize"))
		{
			m_World->Randomize(0.7f, {0, 1, 2, 3});
		}

		if (ImGui::Button("Recenter Camera"))
		{
			m_CameraController.GetCamera().SetPosition({10, 5, -10});
			glm::vec3 forward = glm::normalize(glm::vec3(glm::vec3(0,0,0))  - m_CameraController.GetCamera().GetPosition());
			m_CameraController.GetCamera().SetForwardDirection(forward); ;
		}

		if (ImGui::Button("Add World Floor"))
		{
			m_World->AddWorldFloor(5, 3);
		}

		ImGui::End();
	}

	void OnEvent(Boksi::Event &event) override
	{
	}

private:
	std::shared_ptr<Boksi::Shader> m_Shader;
	std::shared_ptr<Boksi::VertexArray> m_VertexArray;
	Boksi::CameraController m_CameraController;
	std::shared_ptr<Boksi::World> m_World;
	std::shared_ptr<Boksi::Texture2D> m_Texture;
	std::shared_ptr<Boksi::StorageBuffer> m_StorageBuffer;
	std::shared_ptr<Boksi::StorageBuffer> m_MaterialStorageBuffer;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Time = std::chrono::high_resolution_clock::now();
	std::shared_ptr<Boksi::VoxelMeshSVO> m_VoxelMeshSVO;
	std::shared_ptr<Boksi::VoxelRendererSVO> m_VoxelRendererSVO;
	std::shared_ptr<Boksi::VoxelMeshArray> m_VoxelMeshArray;
	glm::vec3 m_LightPosition = {0, 0, 0};
	float m_LastFrameTime = 0.0f;
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