#include "bkpch.h"
#include "EntitiesArray.h"
#include "Cube.h"

namespace Boksi
{
    void EntitiesArray::AddEntity(Ref<Entity> entity)
    {
        m_Entities.push_back(entity);
    }

    void EntitiesArray::ListEntities()
    {
        BK_CORE_TRACE("Entity List");
        for (auto &entity : m_Entities)
        {
            // Do something
            if (typeid(*entity) == typeid(Cube))
            {
                BK_CORE_TRACE("Cube");
            }
            else 
            {
                BK_CORE_WARN("Unknown Entity"); 
            }

            BK_CORE_TRACE("Position: {0}, {1}, {2}", entity->GetPosition().x, entity->GetPosition().y, entity->GetPosition().z);
            BK_CORE_TRACE("Rotation: {0}, {1}, {2}", entity->GetRotation().x, entity->GetRotation().y, entity->GetRotation().z);

        }
    }

    void EntitiesArray::OnUpdate()
    {
        for (auto &entity : m_Entities)
        {
            entity->OnUpdate();
        }
    }

    void EntitiesArray::Draw(Ref<VoxelMesh> mesh)
    {
        for (auto &entity : m_Entities)
        {
            entity->Draw(mesh);
        }
    }
}