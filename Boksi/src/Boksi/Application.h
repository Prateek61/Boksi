#pragma once

#include "Core.h"

namespace Boksi
{
	class BOKSI_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}



