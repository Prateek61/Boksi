#pragma once
#include "bkpch.h"

#include "Boksi/Core/Layer.h"

namespace Boksi
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;
	private:
		float m_Time = 0.0f;
	};
}
