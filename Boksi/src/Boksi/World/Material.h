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
        Material()
	        : m_Color({0.8f, 0.8f, 0.8f}), m_MaterialID(s_MaterialID++) {}

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
        static uint8_t AddMaterial(const Material material, const std::string name)
        {
            s_MaterialIDs[name] = material.GetMaterialID();
            s_Materials[material.GetMaterialID()] = material;
            s_MaterialAdded = true;
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

        static void ResetMaterialAdded() { s_MaterialAdded = false; }
        static bool IsMaterialAdded() { return s_MaterialAdded; }

        static std::vector<glm::vec3> GetColors()
		{
			std::vector<glm::vec3> colors;
            // Pre allocate the size of the vector
            colors.reserve(s_Materials.size());
            for (const auto& material : s_Materials)
			{
				colors.push_back(material.second.GetColor());
			}

            return colors;
		}

    private:
        static std::unordered_map<std::string, uint8_t> s_MaterialIDs;
        static std::map<uint8_t, Material> s_Materials;
        static bool s_MaterialAdded;
    };
}