// Material Class
#pragma once
#include "bkpch.h"

#include <glm/glm.hpp>

namespace Boksi
{
    class Material
    {
    public:
        Material(const glm::vec3& color)
            : m_Color(color), m_MaterialID(s_MaterialID++) {}
        Material();

        const glm::vec3& GetColor() const { return m_Color; }
        const uint8_t GetMaterialID() const { return m_MaterialID; }
    private:
        glm::vec3 m_Color;
        uint8_t m_MaterialID;

    private:
        static uint8_t s_MaterialID;
    };

    // Singleton class to store and retrieve materials
    class MaterialLibrary
    {
    public:
        static uint8_t AddMaterial(Material material, const std::string name)
        {
            s_Materials[material.GetMaterialID()] = std::move(material);
            s_MaterialIDs[name] = material.GetMaterialID();

            return material.GetMaterialID();
        }
        static const Material& GetMaterial(uint8_t materialID)
        {
            return s_Materials[materialID];
        }

        static const Material& GetMaterial(const std::string& name)
        {
            return s_Materials[s_MaterialIDs[name]];
        }
    private:
        static std::unordered_map<std::string, uint8_t> s_MaterialIDs;
        static std::unordered_map<uint8_t, Material> s_Materials;
    };
}