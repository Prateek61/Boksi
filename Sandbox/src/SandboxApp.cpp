#include <Boksi.h>
#include "imgui/imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

const std::string res_path = "Boksi/res/";

class ExampleLayer : public Boksi::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_CameraController(1280.0f, 720.0f)
	{
		// Vertex Array
		m_VertexArray.reset(Boksi::VertexArray::Create());

		// full screen square
		float vertices[] = { 
			-1.0f,
			-1.0f,
			0.0f,

			1.0f,
			-1.0f,
			0.0f,

			1.0f,
			1.0f,
			0.0f,

			-1.0f,
			1.0f,
			0.0f,

		};

		m_VertexBuffer.reset(Boksi::VertexBuffer::Create(vertices, sizeof(vertices)));

		Boksi::BufferLayout layout = {
			{Boksi::ShaderDataType::Float3, "a_Position"},
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// Index Buffer
		uint32_t indices[6] = {
			0, 1, 2, // First triangle
			2, 3, 0	 // Second triangle
		};
		m_IndexBuffer.reset(Boksi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// Shader
		const std::string vertexSrc = Boksi::Renderer::ReadFile(res_path + "Shaders/test.vertex.glsl");

		const std::string fragmentSrc = Boksi::Renderer::ReadFile(res_path +"Shaders/test.fragment.glsl");

		m_Shader.reset(Boksi::Shader::Create(vertexSrc, fragmentSrc));
	}

	void OnUpdate() override
	{

		// Add color to the window
		Boksi::RenderCommand::Clear();
		Boksi::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});


		// Render
		Boksi::Renderer::BeginScene(m_Camera);

		std::dynamic_pointer_cast<Boksi::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", glm::vec4(0.2f, 0.9f, 0.8f, 1.0f));
		m_CameraController.GetCamera().AddToShader(m_Shader);

		Boksi::Renderer::Submit(m_Shader, m_VertexArray);

		Boksi::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::ColorEdit4("Square Color", new float[4]{0.2f, 0.3f, 0.8f, 1.0f});
		ImGui::End();
	}

	void OnEvent(Boksi::Event &event) override
	{

		if (event.GetEventType() == Boksi::EventType::KeyPressed)
		{
			Boksi::KeyPressedEvent &e = (Boksi::KeyPressedEvent &)event;
			if (e.GetKeyCode() == Boksi::Key::W)
			{
				m_CameraController.MoveForward(0.1f);
			}
			if (e.GetKeyCode() == Boksi::Key::S)
			{
				m_CameraController.MoveBackward(0.1f);
			}
			if (e.GetKeyCode() == Boksi::Key::A)
			{
				m_CameraController.MoveLeft(0.1f);
			}
			if (e.GetKeyCode() == Boksi::Key::D)
			{
				m_CameraController.MoveRight(0.1f);
			}

			if (e.GetKeyCode() == Boksi::Key::Up)
			{
				m_CameraController.m_Camera.LookAt += glm::vec3(0.0f, 0.0f, 0.1f);
			}
			if (e.GetKeyCode() == Boksi::Key::Down)
			{
				m_CameraController.m_Camera.LookAt -= glm::vec3(0.0f, 0.0f, 0.1f);
			}
			
			m_CameraController.m_Camera.Update();
		}
		
	}

private:
	std::shared_ptr<Boksi::Shader> m_Shader;
	std::shared_ptr<Boksi::VertexArray> m_VertexArray;
	std::shared_ptr<Boksi::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Boksi::IndexBuffer> m_IndexBuffer;

	Boksi::OrthographicCamera m_Camera;
	Boksi::CameraController m_CameraController;
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