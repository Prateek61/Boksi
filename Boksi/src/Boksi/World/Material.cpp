#include "bkpch.h"
#include "Material.h"

namespace Boksi
{
    uint8_t Material::s_MaterialID = 1;

    std::unordered_map<std::string, uint8_t> MaterialLibrary::s_MaterialIDs;
    std::map<uint8_t, Material> MaterialLibrary::s_Materials;
    bool MaterialLibrary::s_MaterialAdded = true;
}