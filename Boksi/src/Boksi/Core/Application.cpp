#include "bkpch.h"
#include "Application.h"
#include "GLAD/glad.h"

#include "Boksi/Events/ApplicationEvent.h"

#include "Boksi/Renderer/Renderer.h"


#include "Input.h"

#include "GLFW/glfw3.h"

namespace Boksi
{
	Application *Application::s_Instance = nullptr;

	Application::Application()
		: m_Window(Window::Create())
	{
		m_Window->SetVSync(true);
		BK_CORE_ASSERT(!s_Instance, "Application already exists!");

		s_Instance = this;
		m_Window->SetEventCallback(BK_BIND_EVENT_FN(OnEvent));
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = static_cast<float>(glfwGetTime());
			TimeStep time_step = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer *layer : m_LayerStack)
				layer->OnUpdate(time_step);

			m_ImGuiLayer->Begin();
			for (Layer *layer : m_LayerStack)
				layer->OnImGuiRender(time_step);
			m_ImGuiLayer->End();

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
