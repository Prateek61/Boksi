#pragma once
#include "bkpch.h"
#include "Entity.h"
#include "Boksi/World/Mesh/VoxelMesh.h"

namespace Boksi
{
    class EntitiesArray
    {
    public:
        EntitiesArray() = default;
        EntitiesArray(const EntitiesArray &other) = default;
        EntitiesArray(EntitiesArray &&other) = default;
        EntitiesArray &operator=(const EntitiesArray &other) = default;
        EntitiesArray &operator=(EntitiesArray &&other) = default;
        virtual ~EntitiesArray() = default;
        void AddEntity(Ref<Entity> entity);
        void ListEntities();
        virtual void OnUpdate();
        virtual void Draw(Ref<VoxelMesh> mesh);
        std::vector<Ref<Entity>> GetEntities() { return m_Entities; }
    
    private:
        std::vector<Ref<Entity>> m_Entities;

    };
}