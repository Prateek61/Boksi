#pragma once
#include "bkpch.h"

#include "Boksi/Core/Application.h"

extern Boksi::Application* Boksi::CreateApplication();

int main(int argc, char** argv)
{
	Boksi::Log::Init();
	auto app = Boksi::CreateApplication();
	app->Run();
}
