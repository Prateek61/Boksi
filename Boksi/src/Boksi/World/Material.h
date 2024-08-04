// Material Class
#pragma once
#include "bkpch.h"

#include <glm/glm.hpp>

namespace Boksi
{
    struct Material
    {
        Material()
            : Color(glm::vec3(1.0f, 1.0f, 1.0f)),
              Specular(glm::vec3(0.5f, 0.5f, 0.5f)),
              Ambient(glm::vec3(0.1f, 0.1f, 0.1f)) {}
        Material(const glm::vec3 color, const glm::vec3 specular, const glm::vec3 ambient)
            : Color(color), Specular(specular), Ambient(ambient) {}
            
        glm::vec3 Color;
        glm::vec3 Specular;
        glm::vec3 Ambient;
    };

    class MaterialLibrary
    {
    public:
        static uint8_t AddMaterial(const Material material, const std::string name);
        static const Material &GetMaterial(uint8_t materialID);
        static const Material &GetMaterial(const std::string &name);
        static uint8_t GetMaterialID(const std::string &name);
        static void *GetMaterialData() { return s_Materials.data(); }
        static size_t GetMaterialCount() { return s_Materials.size(); }

        static bool s_NewMaterialAdded;

    private:
        static std::vector<Material> s_Materials;
        static std::unordered_map<std::string, uint8_t> s_MaterialIDs;
    };

    // Singleton class to store and retrieve materials
    // class MaterialLibrary
    // {
    // public:
    //     static uint8_t AddMaterial(const Material material, const std::string name)
    //     {
    //         s_MaterialIDs[name] = material.GetMaterialID();
    //         s_Materials[material.GetMaterialID()] = material;
    //         s_MaterialAdded = true;
    //         return material.GetMaterialID();
    //     }

    //     static const Material& GetMaterial(uint8_t materialID)
    //     {
    //         return s_Materials[materialID];
    //     }

    //     static const Material& GetMaterial(const std::string& name)
    //     {
    //         return s_Materials[s_MaterialIDs[name]];
    //     }

    //     static void ResetMaterialAdded() { s_MaterialAdded = false; }
    //     static bool IsMaterialAdded() { return s_MaterialAdded; }

    //     static std::vector<glm::vec3> GetColors()
    // 	{
    // 		std::vector<glm::vec3> colors;
    //         // Pre allocate the size of the vector
    //         colors.reserve(s_Materials.size());
    //         for (const auto& material : s_Materials)
    // 		{
    // 			colors.push_back(material.second.GetColor());
    // 		}

    //         return colors;
    // 	}

    // private:
    //     static std::unordered_map<std::string, uint8_t> s_MaterialIDs;
    //     static std::map<uint8_t, Material> s_Materials;
    //     static bool s_MaterialAdded;
    // };
}