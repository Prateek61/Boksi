#include "bkpch.h"
#include "Material.h"

namespace Boksi
{
    std::vector<Material> MaterialLibrary::s_Materials({{glm::vec3(0.529f, 0.808f, 0.922f), // Light blue color
                                                         glm::vec3(0.05f, 0.05f, 0.05f),    // Low specular component
                                                         glm::vec3(0.3f, 0.3f, 0.5f)}});    // Low ambient component

    std::unordered_map<std::string, MATERIAL_ID_TYPE> MaterialLibrary::s_MaterialIDs({{"Sky", 0}});
    bool MaterialLibrary::s_NewMaterialAdded = true;

    MATERIAL_ID_TYPE MaterialLibrary::AddMaterial(const Material material, const std::string name)
    {
        s_Materials.push_back(material);
        s_MaterialIDs[name] = static_cast<MATERIAL_ID_TYPE>(s_Materials.size()) - 1;
        s_NewMaterialAdded = true;

        if (s_Materials.size() > pow(2, 16))
		{
            BK_CORE_WARN("Material count exceeds 255!: {}", s_Materials.size());
		}

        return s_Materials.size() - 1;
    }
    const Material &MaterialLibrary::GetMaterial(MATERIAL_ID_TYPE materialID)
    {
        BK_CORE_ASSERT(materialID < s_Materials.size(), "Invalid ID!");
        return s_Materials[materialID];
    }
    const Material &MaterialLibrary::GetMaterial(const std::string &name)
    {
        BK_CORE_ASSERT(s_MaterialIDs.find(name) != s_MaterialIDs.end(), "Material not found!")
        return s_Materials[s_MaterialIDs[name]];
    }
    MATERIAL_ID_TYPE MaterialLibrary::GetMaterialID(const std::string &name)
    {
        BK_CORE_ASSERT(s_MaterialIDs.find(name) != s_MaterialIDs.end(), "Material not found!")
        BK_CORE_INFO("Material ID: {0} {1} {2}",s_Materials[s_MaterialIDs[name]].Color.r, s_Materials[s_MaterialIDs[name]].Color.g, s_Materials[s_MaterialIDs[name]].Color.b);
        return s_MaterialIDs[name];
    }
}