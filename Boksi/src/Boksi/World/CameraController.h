#pragma once

#include "bkpch.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Boksi/Renderer/Camera.h"

namespace Boksi
{
	class CameraController
	{
	public:
		CameraController(float screenWidth, float screenHeight)
			: m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
		{
			m_Camera.ScreenSize = glm::vec2(screenWidth, screenHeight);
			m_Camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
			m_Camera.Direction = glm::vec3(0.0f, 0.0f, -1.0f);
			m_Camera.Up = glm::vec3(0.0f, 1.0f, 0.0f);
			m_Camera.FOV = 45.0f;
			m_Camera.FocalLength = 1.0f;
			m_Camera.AspectRatio = screenWidth / screenHeight;

			m_Camera.Update();
		}

		void MoveForward(float delta)
		{
			m_Camera.Position += m_CameraSpeed * delta * m_Camera.Direction;
			m_Camera.Update();
		}

		void MoveBackward(float delta)
		{
			m_Camera.Position -= m_CameraSpeed * delta * m_Camera.Direction;
			m_Camera.Update();
		}

		void MoveLeft(float delta)
		{
			m_Camera.Position -= glm::normalize(glm::cross(m_Camera.Direction, m_Camera.Up)) * m_CameraSpeed * delta;
			m_Camera.Update();
		}

		void MoveRight(float delta)
		{
			m_Camera.Position += glm::normalize(glm::cross(m_Camera.Direction, m_Camera.Up)) * m_CameraSpeed * delta;
			m_Camera.Update();
		}

		void Rotate(float xOffset, float yOffset)
		{
			xOffset *= m_MouseSensitivity;
			yOffset *= m_MouseSensitivity;

			m_Yaw += xOffset;
			m_Pitch += yOffset;

			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			direction.y = sin(glm::radians(m_Pitch));
			direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_Camera.Direction = glm::normalize(direction);

			m_Camera.Update();
		}

		void LookUp(float delta)
		{
			Rotate(0.0f, delta);
		}

		void LookDown(float delta)
		{
			Rotate(0.0f, -delta);
		}

		void LookLeft(float delta)
		{
			Rotate(-delta, 0.0f);
		}

		void LookRight(float delta)
		{
			Rotate(delta, 0.0f);
		}

		void Zoom(float yOffset)
		{
			m_Camera.FOV -= yOffset;
			if (m_Camera.FOV < 1.0f)
				m_Camera.FOV = 1.0f;
			if (m_Camera.FOV > 45.0f)
				m_Camera.FOV = 45.0f;

			m_Camera.Update();
		}

		const Camera& GetCamera() const { return m_Camera; }

	private:
		Camera m_Camera;
		float m_ScreenWidth, m_ScreenHeight;

		float m_CameraSpeed = 2.5f;
		float m_MouseSensitivity = 0.1f;
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
	};
}
