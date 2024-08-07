#pragma once

#include "bkpch.h"

#include "Timestep.h"

#include "Boksi/Events/Event.h"

namespace Boksi
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender(TimeStep ts) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}