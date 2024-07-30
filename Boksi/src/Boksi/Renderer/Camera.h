#pragma once
#include "bkpch.h"

#include <cmath>
#include <glm/glm.hpp>
#include "Boksi/Renderer/Shader.h"

namespace Boksi
{
	class Camera
	{
	public:
		Camera(float verticalFOV, float nearClip, float farClip);

		void UpdateChanges();
		void OnResize(uint32_t width, uint32_t height);

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetView() const { return m_View; }
		const glm::mat4& GetInverseProjection() const { return m_InverseProjection; }
		const glm::mat4& GetInverseView() const { return m_InverseView; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetForwardDirection() const { return m_ForwardDirection; }
		const glm::vec3& GetUpDirection() const { return m_UpDirection; }
		float GetVerticalFOV() const { return m_VerticalFOV; }
		void SetVerticalFOV(float verticalFOV) { m_VerticalFOV = verticalFOV; m_RecalculateProjection = true; }

		void SetPosition(const glm::vec3& position) { m_Position = position; m_Moved = true; }
		void SetForwardDirection(const glm::vec3& forwardDirection) { m_ForwardDirection = forwardDirection; m_Moved = true; }
		void Rotate(const float pitchDelta, const float yawDelta);

	public:
		static void UploadCameraUniformToShader(const Ref<ShaderUniformUploader>& uniformUploader, const Camera& camera);

	private:
		void RecalculateProjection();
		void RecalculateView();

	private:
		glm::mat4 m_Projection{1.0f};
		glm::mat4 m_View{1.0f};
		glm::mat4 m_InverseProjection{ 1.0f };
		glm::mat4 m_InverseView{ 1.0f };

		float m_VerticalFOV;
		float m_NearClip;
		float m_FarClip;

		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_UpDirection{ 0.0f, 0.0f, 0.0f };

		bool m_Moved = false;
		bool m_RecalculateProjection = false;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	};
}


/*
namespace Boksi
{
	struct Camera
	{
		glm::vec3 Position;
		glm::vec3 Up;
		float FOV;
		float FocalLength;
		float AspectRatio;

		// For FPS camera
		glm::vec3 LookAt;
		glm::vec3 Direction;

		// For ray tracing
		glm::vec3 LowerLeftCorner;
		glm::vec3 Horizontal;
		glm::vec3 Vertical;
		float PixelWidth;

		glm::vec2 ScreenSize;

		void Update()
		{
			// Ensure the Direction vector is correctly normalized
			Direction = glm::normalize(LookAt - Position);

			// Calculate orthonormal basis vectors for the camera
			glm::vec3 w = glm::normalize(Direction);
			glm::vec3 u = glm::normalize(glm::cross(Up, w));
			glm::vec3 v = glm::cross(w, u);

			// Calculate the focal length as the distance from the camera to the look-at point
			FocalLength = glm::length(Position - LookAt);

			// Convert the FOV from degrees to radians for the tangent calculation
			float theta = glm::radians(FOV);
			
			float half_height = std::tan(theta / 2.0f) * FocalLength;
			float half_width = AspectRatio * half_height;

			// Calculate the lower-left corner of the view plane
			LowerLeftCorner = Position - half_width * u - half_height * v - FocalLength * w;

			// Calculate the horizontal and vertical spans of the view plane
			Horizontal = 2.0f * half_width * u;
			Vertical = 2.0f * half_height * v;

			// Calculate the width of a single pixel
			PixelWidth = 2.0f * half_width / ScreenSize.x;
		}

		void AddToShader(const Ref<ComputeShader>& shader) const
		{
			shader->UniformUploader->UploadUniformFloat3("u_Camera.Position", Position);
			shader->UniformUploader->UploadUniformFloat3("u_Camera.Direction", Direction);
			shader->UniformUploader->UploadUniformFloat3("u_Camera.Up", Up);
			shader->UniformUploader->UploadUniformFloat("u_Camera.FOV", FOV);
			shader->UniformUploader->UploadUniformFloat("u_Camera.FocalLength", FocalLength);
			shader->UniformUploader->UploadUniformFloat("u_Camera.AspectRatio", AspectRatio);

			shader->UniformUploader->UploadUniformFloat3("u_Camera.LowerLeftCorner", LowerLeftCorner);
			shader->UniformUploader->UploadUniformFloat3("u_Camera.Horizontal", Horizontal);
			shader->UniformUploader->UploadUniformFloat3("u_Camera.Vertical", Vertical);
			shader->UniformUploader->UploadUniformFloat("u_Camera.PixelWidth", PixelWidth);
			shader->UniformUploader->UploadUniformFloat2("u_Camera.ScreenSize", ScreenSize);
		}
	}
*/