#include "bkpch.h"
#include "Application.h"

#include "Boksi/Events/ApplicationEvent.h"
#include "Boksi/Core/Log.h"

Boksi::Application::Application()
{
}

Boksi::Application::~Application()
{
}

void Boksi::Application::Run()
{
	WindowResizeEvent e(1280, 720);
	BK_TRACE(e.ToString());
	BK_TRACE("Hello World!");

	while (true);
}
