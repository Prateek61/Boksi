#include "bkpch.h"
#include "Application.h"
#include "GLAD/glad.h"

#include "Boksi/Events/ApplicationEvent.h"
#include "Boksi/Renderer/Buffer.h"

#include "Input.h"

namespace Boksi
{
	Application *Application::s_Instance = nullptr;

	Application::Application()
		: m_Window(Window::Create())
	{
		BK_CORE_ASSERT(!s_Instance, "Application already exists!");

		s_Instance = this;
		m_Window->SetEventCallback(BK_BIND_EVENT_FN(OnEvent));
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// Vertex Array
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// full screen square
		float vertices[12] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f};



		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// Index Buffer
		uint32_t indices[6] = {
			0, 1, 2, // First triangle
			2, 3, 0	 // Second triangle
		};

		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// Shader
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;

			void main()
			{	
				
				
				v_Position = a_Position * 1.8;


				gl_Position = vec4(v_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// Add color to the window
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			for (Layer *layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();

			for (Layer *layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			// auto [x, y] = Input::GetMousePosition();
			// BK_CORE_TRACE("Mouse Position: {0}, {1}", x, y);

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BK_BIND_EVENT_FN(OnWindowClose));

		// BK_CORE_TRACE("{0}", e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer *layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer *overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent &e)
	{
		m_Running = false;
		return true;
	}
}
