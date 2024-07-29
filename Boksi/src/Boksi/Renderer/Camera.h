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
		glm::vec3 Direction;
		glm::vec3 Up;
		float FOV;
		float FocalLength;
		float AspectRatio;

		// For ray tracing
		glm::vec3 LowerLeftCorner;
		glm::vec3 Horizontal;
		glm::vec3 Vertical;
		float PixelWidth;

		glm::vec2 ScreenSize;

		void Update()
		{
			glm::vec3 w = glm::normalize(Direction);
			glm::vec3 u = glm::normalize(glm::cross(Up, w));
			glm::vec3 v = glm::cross(w, u);

			float theta = FOV * PI / 180.0f;
			float half_height = 2.0f * std::tan(theta / 2.0f) * FocalLength;
			float half_width = AspectRatio * half_height;

			LowerLeftCorner = Position - half_width * u - half_height * v - FocalLength * w;
			Horizontal = 2.0f * half_width * u;
			Vertical = 2.0f * half_height * v;
			PixelWidth = 2.0f * half_width / ScreenSize.x;
		}

		void AddToShader(Ref<Shader>& shader) const
		{
			auto shader_ptr = std::dynamic_pointer_cast<OpenGLShader>(shader);

			shader_ptr->UploadUniformFloat3("u_Camera.Position", Position);
			shader_ptr->UploadUniformFloat3("u_Camera.Direction", Direction);
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