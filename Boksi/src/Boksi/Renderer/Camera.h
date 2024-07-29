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

			// Display the camera's position and look-at point for debugging
			BK_CORE_TRACE("Look from: {0} , {1} , {2}", Position.x, Position.y, Position.z);
			BK_CORE_TRACE("Look at: {0} , {1} , {2}", LookAt.x, LookAt.y, LookAt.z);
		}

		void AddToShader(Ref<Shader> &shader) const
		{
			auto shader_ptr = std::dynamic_pointer_cast<OpenGLShader>(shader);

			shader_ptr->UploadUniformFloat3("u_Camera.Position", Position);
			shader_ptr->UploadUniformFloat3("u_Camera.LookAt", LookAt);
			shader_ptr->UploadUniformFloat3("u_Camera.Up", Up);
			shader_ptr->UploadUniformFloat("u_Camera.FOV", FOV);
			shader_ptr->UploadUniformFloat("u_Camera.FocalLength", FocalLength);
			shader_ptr->UploadUniformFloat("u_Camera.AspectRatio", AspectRatio);

			shader_ptr->UploadUniformFloat3("u_Camera.LowerLeftCorner", LowerLeftCorner);
			shader_ptr->UploadUniformFloat3("u_Camera.Horizontal", Horizontal);
			shader_ptr->UploadUniformFloat3("u_Camera.Vertical", Vertical);
			shader_ptr->UploadUniformFloat("u_Camera.PixelWidth", PixelWidth);
			shader_ptr->UploadUniformFloat2("u_Camera.ScreenSize", ScreenSize);
		}
	};
}