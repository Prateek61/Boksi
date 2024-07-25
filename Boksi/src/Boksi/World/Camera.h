#pragma once
#include "bkpch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Boksi
{
    // Class representing a camera in the world
    class Camera
    {
    public:
        Camera();
        ~Camera();

        void SetPosition(const glm::vec3& position) { m_Data.Position = position; }
        void SetDirection(const glm::vec3& direction) { m_Data.Direction = direction; }
        void SetUp(const glm::vec3& up) { m_Data.Up = up; }
        void SetFOV(float fov) { m_Data.FOV = fov; }
        void SetAspectRatio(float aspectRatio) { m_Data.AspectRatio = aspectRatio; }
        void SetNearClip(float nearClip) { m_Data.NearClip = nearClip; }
        void SetFarClip(float farClip) { m_Data.FarClip = farClip; }

        const glm::vec3& GetPosition() const { return m_Data.Position; }
        const glm::vec3& GetDirection() const { return m_Data.Direction; }
        const glm::vec3& GetUp() const { return m_Data.Up; }
        float GetFOV() const { return m_Data.FOV; }
        float GetAspectRatio() const { return m_Data.AspectRatio; }
        float GetNearClip() const { return m_Data.NearClip; }
        float GetFarClip() const { return m_Data.FarClip; }

        inline glm::mat4 GetViewMatrix() const;
        inline glm::mat4 GetProjectionMatrix() const;
    private:
        struct CameraData
        {
            glm::vec3 Position;
            glm::vec3 Direction;
            glm::vec3 Up;
            float FOV;
            float AspectRatio;
            float NearClip;
            float FarClip;
        } m_Data;
    public:
    };
}

// Inline definitions
namespace Boksi
{
    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(m_Data.Position, m_Data.Position + m_Data.Direction, m_Data.Up);
    }

    glm::mat4 Camera::GetProjectionMatrix() const
    {
        return glm::perspective(m_Data.FOV, m_Data.AspectRatio, m_Data.NearClip, m_Data.FarClip);
    }
}