#include <Boksi.h>

class Sandbox : public Boksi::Application
{
public:
	Sandbox() {};
	~Sandbox() {};
};

Boksi::Application* Boksi::CreateApplication()
{
	return new Sandbox();
}