#pragma once

#include "Boksi/Core/Base.h"
#include "Boksi/Debug/Instrumentor.h"

namespace Boksi
{
	/// Event type that can be handled by the application
	enum class EventType
	{
		//None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};


	/// Event categories represented by bit fields.\n
	///	Such that an event can be in multiple categories.
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};


	/// Macros to be used in the event classes
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetName() const override { return #type; }
	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	/// Base Event class that can be inherited by other events
	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;

		virtual const char* GetName() const = 0;

		/// Returns the categories that the event is in using bit fields
		virtual int GetCategoryFlags() const = 0;

		virtual std::string ToString() const
		{
			return GetName();
		}

		/// Returns true if the event is in the specified category
		bool IsInCategory(const EventCategory category) const
		{
			return GetCategoryFlags() & category;
		}

		/// To be set to true if the event has been handled
		bool Handled = false;
	};


	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		// F will be deduced by the compiler
		template <typename T, typename F>
		bool Dispatch(const F& func)
		{
			BK_PROFILE_FUNCTION();

			if ( m_Event.GetEventType() == T::GetStaticType() )
			{
				// Call the function with the event and bitwise OR the result with Handled
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
