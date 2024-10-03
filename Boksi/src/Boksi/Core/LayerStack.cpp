#include "bkpch.h"
#include "LayerStack.h"

namespace Boksi
{
	LayerStack::~LayerStack()
	{
		BK_PROFILE_FUNCTION();

		for ( Layer* layer : m_Layers )
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		BK_PROFILE_FUNCTION();

		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		BK_PROFILE_FUNCTION();

		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		BK_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if ( it != m_Layers.begin() + m_LayerInsertIndex )
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		BK_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if ( it != m_Layers.end() )
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}

}
