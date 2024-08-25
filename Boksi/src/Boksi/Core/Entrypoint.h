#pragma once
#include "bkpch.h"

#include "Boksi/Core/Application.h"
#include "Boksi/Events/ApplicationEvent.h"

extern Boksi::Application* Boksi::CreateApplication();

int main(int argc, char** argv)
{
	Boksi::Log::Init();

	BK_PROFILE_BEGIN_SESSION("Startup", "BoksiProfile-Startup.profile.json");
	auto app = Boksi::CreateApplication();
	BK_PROFILE_END_SESSION();

	BK_PROFILE_BEGIN_SESSION("Runtime", "BoksiProfile-Runtime.profile.json");
	app->Run();
	BK_PROFILE_END_SESSION();

	BK_PROFILE_BEGIN_SESSION("Shutdown", "BoksiProfile-Shutdown.profile.json");
	delete app;
	BK_PROFILE_END_SESSION();
}
