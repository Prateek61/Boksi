#pragma once
#include "bkpch.h"

#include <cmath>
#include <glm/glm.hpp>
#include "Boksi/Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

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

			// // Display the camera's position and look-at point for debugging
			// BK_CORE_TRACE("Look from: {0} , {1} , {2}", Position.x, Position.y, Position.z);
			// BK_CORE_TRACE("Look at: {0} , {1} , {2}", LookAt.x, LookAt.y, LookAt.z);
			// BK_CORE_TRACE("Direction: {0} , {1} , {2}", Direction.x, Direction.y, Direction.z);
			// BK_CORE_TRACE("Focal Length: {0}", FocalLength);
			// BK_CORE_TRACE("Lower Left Corner: {0} , {1} , {2}", LowerLeftCorner.x, LowerLeftCorner.y, LowerLeftCorner.z);
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
	};
}