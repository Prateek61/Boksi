#include "bkpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Boksi
{
	Camera::Camera(float verticalFOV, float nearClip, float farClip)
	
		: m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
	{
		m_ForwardDirection = glm::vec3(0.0f, 0.0f, 1.0f);
		m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		m_Moved = true;
		m_RecalculateProjection = true;
	}

	void Camera::UpdateChanges()
	{
		if (m_Moved)
		{
			RecalculateView();
			m_Moved = false;
		}

		if (m_RecalculateProjection)
		{
			RecalculateProjection();
			m_RecalculateProjection = false;
		}
	}
	void Camera::OnResize(uint32_t width, uint32_t height)
	{
		if (width == m_ViewportWidth && height == m_ViewportHeight)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_RecalculateProjection = true;
	}

	void Camera::Rotate(const float pitchDelta, const float yawDelta)
	{
		const glm::vec3 right_direction = glm::cross(m_ForwardDirection, m_UpDirection);
		const glm::quat q = glm::normalize(glm::cross(
			glm::angleAxis(-pitchDelta, right_direction),
			glm::angleAxis(-yawDelta, m_UpDirection)
		));
		m_ForwardDirection = glm::normalize(q * m_ForwardDirection);

		m_Moved = true;
	}

	void Camera::UploadCameraUniformToShader(const Ref<ShaderUniformUploader>& uniformUploader, const Camera& camera)
	{
		uniformUploader->UploadUniformMat4("u_Camera.InverseProjection", camera.GetInverseProjection());
		uniformUploader->UploadUniformMat4("u_Camera.InverseView", camera.GetInverseView());
		uniformUploader->UploadUniformFloat3("u_Camera.Position", camera.GetPosition());
		uniformUploader->UploadUniformFloat3("u_Camera.ForwardDirection", camera.GetForwardDirection());
		uniformUploader->UploadUniformFloat2("u_Camera.ScreenSize", { camera.m_ViewportWidth, camera.m_ViewportHeight });
	}

	void Camera::RecalculateProjection()
	{
		m_Projection = glm::perspectiveFov(
			glm::radians(m_VerticalFOV),
			static_cast<float>(m_ViewportWidth),
			static_cast<float>(m_ViewportHeight),
			m_NearClip,
			m_FarClip
		);
		m_InverseProjection = glm::inverse(m_Projection);
	}

	void Camera::RecalculateView()
	{
		m_View = glm::lookAt(
			m_Position,
			m_Position + m_ForwardDirection,
			m_UpDirection
		);
		m_InverseView = glm::inverse(m_View);
	}
}