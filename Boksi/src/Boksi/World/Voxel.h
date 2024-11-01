#pragma once

#include "Boksi/World/Material.h"
#include "Boksi/Core/MathUtils.h"

#include <cstdint>
#include <cstring>

namespace Boksi
{
#define EMPTY_VOXEL 0ull

    // Old Class representing a single voxel in the voxel world
    using VoxelOld = uint16_t;

    constexpr uint32_t VOXEL_SIZE = 8;

	// Class representing a single voxel in the voxel world
    class Voxel
    {
    public:
        Voxel();
        Voxel(uint64_t data);
        Voxel(MATERIAL_ID_TYPE materialID, const glm::vec3& albedo, const glm::vec3& normal);
        Voxel(MATERIAL_ID_TYPE materialID, uint8_t albedo[3], uint16_t normal);
        Voxel(const Voxel& other) = default;

        // Getters
        glm::vec3 GetAlbedoVec3() const;
		glm::vec3 GetNormalVec3() const;

        // Setters
		void SetAlbedoVec3(const glm::vec3& albedo);
        void SetNormalVec3(const glm::vec3& normal);

        ~Voxel() = default;

    public:
        union
        {
	        struct
	        {
                MATERIAL_ID_TYPE MaterialID;
                union
                {
	                struct
	                {
                        uint8_t AlbedoR, AlbedoG, AlbedoB;
	                };
                    uint8_t AlbedoData[3];
                };
                uint16_t Normal;
                uint8_t Padding;
	        };
            uint64_t Data;
        };
    };
}

// Definitions
namespace Boksi
{
	inline Voxel::Voxel()
		: MaterialID(0), AlbedoR(0), AlbedoG(0), AlbedoB(0), Normal(0)
	{
	}

    inline Voxel::Voxel(uint64_t data)
	    : Data(data)
	{
	}

    inline Voxel::Voxel(MATERIAL_ID_TYPE materialID, const glm::vec3& albedo, const glm::vec3& normal)
	    : MaterialID(materialID)
	{
		SetAlbedoVec3(albedo);
        SetNormalVec3(normal);
	}

    inline Voxel::Voxel(MATERIAL_ID_TYPE materialID, uint8_t albedo[3], uint16_t normal)
		: MaterialID(materialID), AlbedoR(albedo[0]), AlbedoG(albedo[1]), AlbedoB(albedo[2]), Normal(normal)
	{
	}

    inline glm::vec3 Voxel::GetAlbedoVec3() const
	{
        return { static_cast<float>(AlbedoR) / 255.0f, static_cast<float>(AlbedoG) / 255.0f, static_cast<float>(AlbedoB) / 255.0f };
	}

    inline glm::vec3 Voxel::GetNormalVec3() const
	{
        glm::vec2 octahedral = { static_cast<float>(Normal & 0xFF) / 255.0f, static_cast<float>(Normal >> 8) / 255.0f };
        return Maths::OctahedralDecode(octahedral);
	}

    inline void Voxel::SetAlbedoVec3(const glm::vec3& albedo)
	{
		AlbedoR = static_cast<uint8_t>(albedo.r * 255.0f);
		AlbedoG = static_cast<uint8_t>(albedo.g * 255.0f);
		AlbedoB = static_cast<uint8_t>(albedo.b * 255.0f);
	}

	inline void Voxel::SetNormalVec3(const glm::vec3& normal)
	{
		glm::vec2 octahedral = Maths::OctahedralEncode(glm::normalize(normal));
		Normal = static_cast<uint16_t>(static_cast<uint16_t>(octahedral.x * 255.0f) | (static_cast<uint16_t>(octahedral.y * 255.0f) << 8));
	}
}
