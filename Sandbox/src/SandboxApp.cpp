#include <Boksi.h>
#include "imgui/imgui/imgui.h"

const std::string res_path = "Boksi/res/";

class ExampleLayer : public Boksi::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),
		  m_CameraController(1280.0f, 720.0f),
		  m_World(new Boksi::World({64, 64, 64}))
	{
		AttachShadersAndBuffers();

		// Randomize the world
		m_World->Randomize(0.5f, {0, 1 ,2 ,3});

		// Set the clear color
		Boksi::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
	}

	void AttachShadersAndBuffers()
	{
		// Compute Shader
		const std::string compute_src = Boksi::Renderer::ReadFile(res_path + "Shaders/ray_trace.comp.glsl");
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

		// Storage Buffer
		uint32_t size = m_World->GetVoxelCount() * sizeof(Boksi::Voxel);
		m_StorageBuffer.reset(Boksi::StorageBuffer::Create(size));
	}

	void OnUpdate() override
	{
		Boksi::RenderCommand::Clear();

		// Render
		Boksi::Renderer::BeginScene();

		// Compute Shader
		m_ComputeShader->Bind();
		// Set the SSBO
		m_StorageBuffer->SetData(m_World->GetVoxelsData(), m_World->GetVoxelCount() * sizeof(Boksi::Voxel));
		// Bind SSBO
		m_StorageBuffer->Bind(1);
		// Camera Uniforms
		m_CameraController.GetCamera().AddToShader(m_ComputeShader);
		// Bind the texture
		m_Texture->BindWrite(0);
		// Dispatch Compute Shader
		Boksi::Renderer::DispatchCompute(m_ComputeShader, 1280 / 16, 720 / 16, 1);

		// Check for errors
		Boksi::RenderCommand::CheckForErrors();

		// Render the texture to the screen
		m_Texture->Bind(0);
		m_Shader->Bind();
		m_Shader->UniformUploader->UploadUniformInt("screenTexture", 0);
		Boksi::Renderer::Submit(m_Shader, m_VertexArray);

		Boksi::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		// ImGui::Begin("Test");
		// ImGui::Text("Hello World!");
		// ImGui::ColorEdit4("Square Color", new float[4]{0.2f, 0.3f, 0.8f, 1.0f});
		// ImGui::End();
	}

	void OnEvent(Boksi::Event &event) override
	{
		const float camera_speed = 0.01f;

		if (event.GetEventType() == Boksi::EventType::KeyPressed)
		{
			Boksi::KeyPressedEvent &e = (Boksi::KeyPressedEvent &)event;
			if (e.GetKeyCode() == Boksi::Key::W)
			{
				m_CameraController.MoveForward(camera_speed);
			}
			if (e.GetKeyCode() == Boksi::Key::S)
			{
				m_CameraController.MoveBackward(camera_speed);
			}
			if (e.GetKeyCode() == Boksi::Key::A)
			{
				m_CameraController.MoveLeft(camera_speed);
			}
			if (e.GetKeyCode() == Boksi::Key::D)
			{
				m_CameraController.MoveRight(camera_speed);
			}

			// Update the camera
			if (e.GetKeyCode() == Boksi::Key::Up)
			{
				m_CameraController.m_Camera.LookAt += glm::vec3(camera_speed, 0.0f, 0.0f);
			}
			if (e.GetKeyCode() == Boksi::Key::Down)
			{
				m_CameraController.m_Camera.LookAt -= glm::vec3(camera_speed, 0.0f, 0.0f);
			}
			if (e.GetKeyCode() == Boksi::Key::Left)
			{
				m_CameraController.m_Camera.LookAt += glm::vec3(0.0f, camera_speed, 0.0f);
			}
			if (e.GetKeyCode() == Boksi::Key::Right)
			{
				m_CameraController.m_Camera.LookAt -= glm::vec3(0.0f, camera_speed, 0.0f);
			}



			m_CameraController.m_Camera.Update();
		}
	}

private:
	std::shared_ptr<Boksi::Shader> m_Shader;
	std::shared_ptr<Boksi::VertexArray> m_VertexArray;
	std::shared_ptr<Boksi::ComputeShader> m_ComputeShader;
	Boksi::CameraController m_CameraController;
	std::shared_ptr<Boksi::World> m_World;
	std::shared_ptr<Boksi::Texture2D> m_Texture;
	std::shared_ptr<Boksi::StorageBuffer> m_StorageBuffer;
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