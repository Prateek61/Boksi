#include "bkpch.h"
#include "CameraController.h"

#include "Boksi/Core/Input.h"
#include "Boksi/Core/KeyCodes.h"
#include "Boksi/Core/MouseCodes.h"
#include "Boksi/Core/Timestep.h"

namespace Boksi
{
	void CameraController::OnUpdate(TimeStep ts)
	{
		HandleInput(ts);
		m_Camera.UpdateChanges();
	}

	void CameraController::HandleInput(TimeStep ts)
	{
		glm::vec3 right_direction = glm::cross(m_Camera.GetForwardDirection(), m_Camera.GetUpDirection());
		glm::vec3 up_direction = m_Camera.GetUpDirection();

		if (Input::IsKeyPressed(Key::W))
		{
			m_Camera.SetPosition(m_Camera.GetPosition() + m_Camera.GetForwardDirection() * m_CameraMoveSpeed * ts.GetMilliseconds());
		}
		if (Input::IsKeyPressed(Key::S))
		{
			m_Camera.SetPosition(m_Camera.GetPosition() - m_Camera.GetForwardDirection() * m_CameraMoveSpeed * ts.GetMilliseconds());
		}
		if (Input::IsKeyPressed(Key::A))
		{
			m_Camera.SetPosition(m_Camera.GetPosition() - right_direction * m_CameraMoveSpeed * ts.GetMilliseconds());
		}
		if (Input::IsKeyPressed(Key::D))
		{
			m_Camera.SetPosition(m_Camera.GetPosition() + right_direction * m_CameraMoveSpeed * ts.GetMilliseconds());
		}
		if (Input::IsKeyPressed(Key::Q))
		{
			m_Camera.SetPosition(m_Camera.GetPosition() + m_Camera.GetUpDirection() * m_CameraMoveSpeed * ts.GetMilliseconds());
		}
		if (Input::IsKeyPressed(Key::E))
		{
			m_Camera.SetPosition(m_Camera.GetPosition() - m_Camera.GetUpDirection() * m_CameraMoveSpeed * ts.GetMilliseconds());
		}

		// Rotation
		auto [x, y] = Input::GetMousePosition();
		const glm::vec2 mouse_position = { x, y };
		const glm::vec2 delta = mouse_position - m_LastMousePosition;
		m_LastMousePosition = mouse_position;

		if ((delta.x != 0.0f || delta.y != 0.0f) && Input::IsMouseButtonPressed(Mouse::Button0))
		{
			m_Camera.Rotate(delta.y * m_CameraMouseSensitivity * ts.GetMilliseconds(), delta.x * m_CameraMouseSensitivity * ts.GetMilliseconds());
		}
	}
}
