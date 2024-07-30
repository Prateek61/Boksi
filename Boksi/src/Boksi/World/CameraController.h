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
		CameraController(float verticalFOV, float nearClip, float farClip)
			: m_Camera(verticalFOV, nearClip, farClip)
		{}

		void OnUpdate(float deltaTime);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }
		float GetCameraMoveSpeed() const { return m_CameraMoveSpeed; }
		float GetCameraMouseSensitivity() const { return m_CameraMouseSensitivity; }

		void SetCameraMoveSpeed(float cameraMoveSpeed) { m_CameraMoveSpeed = cameraMoveSpeed; }
		void SetCameraMouseSensitivity(float cameraMouseSensitivity) { m_CameraMouseSensitivity = cameraMouseSensitivity; }

	private:
		void HandleInput(float deltaTime);

	private:
		Camera m_Camera;
		float m_CameraMoveSpeed = 5.0f;
		float m_CameraMouseSensitivity = 0.1f;
		glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };
	};
}
