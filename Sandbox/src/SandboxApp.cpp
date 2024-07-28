#include <Boksi.h>
#include "imgui/imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Boksi::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{

		// Vertex Array
		m_VertexArray.reset(Boksi::VertexArray::Create());

		// full screen square
		float vertices[3 * (3 + 4)] = {
			-0.5f,
			-0.5f,
			0.0f,
			1.0f,
			0.0f,
			1.0f,
			1.0f,
			0.5f,
			-0.5f,
			0.0f,
			0.0f,
			0.0f,
			1.0f,
			1.0f,
			0.0f,
			0.5f,
			0.0f,
			1.0f,
			1.0f,
			0.0f,
			1.0f,
		};

		m_VertexBuffer.reset(Boksi::VertexBuffer::Create(vertices, sizeof(vertices)));

		Boksi::BufferLayout layout = {
			{Boksi::ShaderDataType::Float3, "a_Position"},
			{Boksi::ShaderDataType::Float4, "a_Color"}};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// Index Buffer
		uint32_t indices[3] = {
			0, 1, 2, // First triangle
		};
		m_IndexBuffer.reset(Boksi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// Shader
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{	
				v_Position = a_Position;
				v_Color = a_Color;
				
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			uniform vec4 u_Color;

			void main()
			{

				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = u_Color;
			}
		)";

		m_Shader.reset(Boksi::Shader::Create(vertexSrc, fragmentSrc));
	}

	void OnUpdate() override
	{

		// Add color to the window
		Boksi::RenderCommand::Clear();
		Boksi::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});

		m_Camera.SetRotation(45.0f);

		// Render
		Boksi::Renderer::BeginScene(m_Camera);

		std::dynamic_pointer_cast<Boksi::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", glm::vec4(0.2f, 0.9f, 0.8f, 1.0f));

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
			if (e.GetKeyCode() == Boksi::Key::Tab)
				BK_TRACE("Tab key is pressed (event)!");
			BK_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

private:
	std::shared_ptr<Boksi::Shader> m_Shader;
	std::shared_ptr<Boksi::VertexArray> m_VertexArray;
	std::shared_ptr<Boksi::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Boksi::IndexBuffer> m_IndexBuffer;

	Boksi::OrthographicCamera m_Camera;
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