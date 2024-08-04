#include "bkpch.h"
#include "Material.h"

namespace Boksi
{
    std::vector<Material> MaterialLibrary::s_Materials({{glm::vec3(0.529f, 0.808f, 0.922f), // Light blue color
                                                         glm::vec3(0.05f, 0.05f, 0.05f),    // Low specular component
                                                         glm::vec3(0.3f, 0.3f, 0.5f)}});    // Low ambient component

    std::unordered_map<std::string, uint8_t> MaterialLibrary::s_MaterialIDs({{"Sky", 0}});
    bool MaterialLibrary::s_NewMaterialAdded = true;

    uint8_t MaterialLibrary::AddMaterial(const Material material, const std::string name)
    {
        s_Materials.push_back(material);
        s_MaterialIDs[name] = static_cast<uint8_t>(s_Materials.size()) - 1;
        return s_Materials.size() - 1;
        s_NewMaterialAdded = true;
    }
    const Material &MaterialLibrary::GetMaterial(uint8_t materialID)
    {
        BK_CORE_ASSERT(materialID < s_Materials.size(), "Invalid ID!");
        return s_Materials[materialID];
    }
    const Material &MaterialLibrary::GetMaterial(const std::string &name)
    {
        BK_CORE_ASSERT(s_MaterialIDs.find(name) != s_MaterialIDs.end(), "Material not found!")
        return s_Materials[s_MaterialIDs[name]];
    }
    uint8_t MaterialLibrary::GetMaterialID(const std::string &name)
    {
        BK_CORE_ASSERT(s_MaterialIDs.find(name) != s_MaterialIDs.end(), "Material not found!")
        BK_CORE_INFO("Material ID: {0} {1} {2}",s_Materials[s_MaterialIDs[name]].Color.r, s_Materials[s_MaterialIDs[name]].Color.g, s_Materials[s_MaterialIDs[name]].Color.b);
        return s_MaterialIDs[name];
    }
}