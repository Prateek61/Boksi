#pragma once

#include "Boksi/Core/Layer.h"

namespace Boksi
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender(TimeStep ts) override;

		void Begin();
		void End();
	
	private:
		float m_Time = 0.0f;
	};
}
