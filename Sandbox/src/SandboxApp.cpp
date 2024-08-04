#include <Boksi.h>
#include "imgui/imgui/imgui.h"
#include <chrono>

const std::string res_path = "../Boksi/res/";

const int Power = 6;
const int WORLD_SIZE = 256;

class ExampleLayer : public Boksi::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),
		  m_CameraController(45.0f, 0.1f, 100.0f),
		  m_World(new Boksi::World({WORLD_SIZE, WORLD_SIZE, WORLD_SIZE}, EMPTY_VOXEL)),
		  m_VoxelRendererSVO(new Boksi::VoxelRendererSVO(res_path + "Shaders/ray_trace_svo.glsl"))
	{
		m_World->AddWorldFloor(2, 1);
		// m_World->Randomize(0.5f, {1});
		m_VoxelMeshSVO.reset(new Boksi::VoxelMeshSVO(m_World->GetVoxels(), m_World->GetSize()));

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

		m_World->DrawCircle(30, 10, 10, 5, 1);
		m_World->DrawCircle(50, 10, 10, 5, 1);
		m_CameraController.GetCamera().SetPosition({30, 0, -30});
	}

	void AttachShadersAndBuffers()
	{
		// Compute Shader
		const std::string compute_src = Boksi::ShaderLoader::Load(res_path + "Shaders/ray_trace_original.comp.glsl");
		m_ComputeShader.reset(Boksi::ComputeShader::Create(compute_src));

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

		// Send Dimensions as Unfiorm
		m_ComputeShader->Bind();
		m_ComputeShader->UniformUploader->UploadUniformInt3("u_Dimensions", {m_World->GetSize().x, m_World->GetSize().y, m_World->GetSize().z});

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

		// Check for errors
		m_VoxelRendererSVO->Render(m_CameraController.GetCamera(), m_Texture, m_VoxelMeshSVO->GetSize(), m_VoxelMeshSVO->GetMaximumDepth(), 0.5f, m_VoxelMeshSVO);
		
		Boksi::RenderCommand::CheckForErrors();
		
		// Render the texture to the screen
		m_Texture->Bind(0);
		m_Shader->Bind();
		m_Shader->UniformUploader->UploadUniformInt("screenTexture", 0);
		Boksi::Renderer::Submit(m_Shader, m_VertexArray);


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
			m_World.reset(new Boksi::World({x, y, z}));
			m_World->Randomize(0.1f, {0, 1, 2, 3});

			AttachShadersAndBuffers();

			m_ComputeShader->Bind();
			m_ComputeShader->UniformUploader->UploadUniformInt3("u_Dimensions", {m_World->GetSize().x, m_World->GetSize().y, m_World->GetSize().z});
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
			m_CameraController.GetCamera().SetPosition({0, 0, -10});
		}

		if (ImGui::Button("Add World Floor"))
		{
			m_World->AddWorldFloor(5, 3);
		}

		if (ImGui::Button("Draw Circle"))
		{

			glm::vec3 cameraLocation = m_CameraController.GetCamera().GetPosition() + m_CameraController.GetCamera().GetForwardDirection() * 10.0f;

			glm::vec3 voxelPos = glm::vec3(floor(cameraLocation.x), floor(cameraLocation.y), floor(cameraLocation.z));

			if (voxelPos.x < 0 || voxelPos.x >= m_World->GetSize().x || voxelPos.y < 0 || voxelPos.y >= m_World->GetSize().y || voxelPos.z < 0 || voxelPos.z >= m_World->GetSize().z)
			{
				BK_ERROR("Out of bounds UwU");
			}
			else
			{
				BK_TRACE("Drawing circle at: ({}, {}, {})", voxelPos.x, voxelPos.y, voxelPos.z);
				m_World->DrawCircle(voxelPos.x, voxelPos.y, voxelPos.z, 5, 2);
			}
		}

		ImGui::End();
	}

	void OnEvent(Boksi::Event &event) override
	{
	}

private:
	std::shared_ptr<Boksi::Shader> m_Shader;
	std::shared_ptr<Boksi::VertexArray> m_VertexArray;
	std::shared_ptr<Boksi::ComputeShader> m_ComputeShader;
	Boksi::CameraController m_CameraController;
	std::shared_ptr<Boksi::World> m_World;
	std::shared_ptr<Boksi::Texture2D> m_Texture;
	std::shared_ptr<Boksi::StorageBuffer> m_StorageBuffer;
	std::shared_ptr<Boksi::StorageBuffer> m_MaterialStorageBuffer;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Time = std::chrono::high_resolution_clock::now();
	std::shared_ptr<Boksi::VoxelMeshSVO> m_VoxelMeshSVO;
	std::shared_ptr<Boksi::VoxelRendererSVO> m_VoxelRendererSVO;
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