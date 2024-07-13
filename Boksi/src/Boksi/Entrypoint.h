#pragma once

extern Boksi::Application* Boksi::CreateApplication();

int main(int argc, char** argv)
{
	Boksi::Log::Init();
	BK_CORE_WARN("Initialized Log!");
	int a = 5;
	BK_INFO("Hello! Var={0}", a);
}
