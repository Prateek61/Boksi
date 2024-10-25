#pragma once

#include "Boksi.h"

class SandboxLayer final : public Boksi::Layer
{
public:
	SandboxLayer() : Layer("SandboxLayer") {}

	void OnAttach() override;
	void OnUpdate(Boksi::TimeStep ts) override;
};