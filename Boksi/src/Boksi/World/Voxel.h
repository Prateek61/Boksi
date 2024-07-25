#pragma once
#include "bkpch.h"

#include <glm/glm.hpp>

namespace Boksi
{
    // Class representing a single voxel in the voxel world
    class Voxel
    {
    public:
        Voxel(const uint8_t materialID)
            : m_MaterialID(materialID), m_Filled(true) {}
        Voxel()
            : m_MaterialID(0), m_Filled(false) {}

        // Getters
        uint8_t GetMaterialID() const { return m_MaterialID; }
        bool IsFilled() const { return m_Filled; }

        // Setters
        void SetMaterialID(const uint8_t materialID) { m_MaterialID = materialID; m_Filled = true; }
        void Unfill() { m_Filled = false; m_MaterialID = 0; }

    private:
        uint8_t m_MaterialID;
        bool m_Filled;
    };
}
