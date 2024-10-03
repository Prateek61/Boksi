#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

namespace Boksi
{
    using MATERIAL_ID_TYPE = uint16_t;

    struct Material
    {
        Material()
            : Color(glm::vec3(1.0f, 1.0f, 1.0f)),
              Specular(glm::vec3(0.5f, 0.5f, 0.5f)),
              Ambient(glm::vec3(0.1f, 0.1f, 0.1f)) {}
        Material(const glm::vec3 color, const glm::vec3 specular, const glm::vec3 ambient)
            : Color(color), Specular(specular), Ambient(ambient) {}
            
        glm::vec3 Color;
        float padding1;
        glm::vec3 Specular;
        float padding2;
        glm::vec3 Ambient;
        float padding3;
    };

    class MaterialLibrary
    {
    public:
        static MATERIAL_ID_TYPE AddMaterial(const Material material, const std::string name);
        static const Material &GetMaterial(MATERIAL_ID_TYPE materialID);
        static const Material &GetMaterial(const std::string &name);
        static MATERIAL_ID_TYPE GetMaterialID(const std::string &name);
        static void *GetMaterialData() { return s_Materials.data(); }
        static size_t GetMaterialCount() { return s_Materials.size(); }

        static bool s_NewMaterialAdded;

    private:
        static std::vector<Material> s_Materials;
        static std::unordered_map<std::string, MATERIAL_ID_TYPE> s_MaterialIDs;
    };
}