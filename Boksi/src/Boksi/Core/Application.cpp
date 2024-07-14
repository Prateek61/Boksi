#include "bkpch.h"
#include "Application.h"

#include "Boksi/Events/ApplicationEvent.h"
#include "Boksi/Core/Log.h"

#include <GLFW/glfw3.h>

Boksi::Application::Application()
	: m_Window(Boksi::Window::Create())
{
}

Boksi::Application::~Application()
{
}

void Boksi::Application::Run()
{
	while (m_Running)
	{
		m_Window->OnUpdate();
	}
}
