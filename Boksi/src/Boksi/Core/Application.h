#pragma once
#include "bkpch.h"

#include "Boksi/Core/Window.h"
#include "Boksi/Core/LayerStack.h"
#include "Boksi/Events/ApplicationEvent.h"
#include "Boksi/Imgui/ImGuiLayer.h"
#include "Boksi/Renderer/Shader.h"
#include "Boksi/Renderer/Buffer.h"

namespace Boksi
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(Event &e);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);

		inline Window &GetWindow();
		inline static Application &Get();

	private:
		Scope<Window> m_Window;
		ImGuiLayer *m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent &e);
		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

	private:
		static Application *s_Instance;
	};

	// To be defined in CLIENT
	Application *CreateApplication();
}

namespace Boksi
{
	Window &Application::GetWindow()
	{
		return *m_Window;
	}

	Application &Application::Get()
	{
		return *s_Instance;
	}
}
