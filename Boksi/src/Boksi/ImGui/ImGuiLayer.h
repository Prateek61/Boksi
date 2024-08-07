#pragma once
#include "bkpch.h"

#include "Boksi/Core/Layer.h"
#include "Boksi/Events/ApplicationEvent.h"
#include "Boksi/Events/KeyEvent.h"
#include "Boksi/Events/MouseEvent.h"

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
