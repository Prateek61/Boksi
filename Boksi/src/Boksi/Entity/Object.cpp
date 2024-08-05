#include "bkpch.h"
#include "Object.h"
#include "Boksi/World/ModelLoader.h"
#include "Boksi/World/Material.h"

namespace Boksi
{

    struct Vec3Comparator
    {
        bool operator()(const glm::vec3 &lhs, const glm::vec3 &rhs) const
        {
            if (lhs.x != rhs.x)
                return lhs.x < rhs.x;
            if (lhs.y != rhs.y)
                return lhs.y < rhs.y;
            return lhs.z < rhs.z;
        }
    };

    Object::Object(glm::vec3 position, glm::vec3 rotation, size_t size, std::string name = "Unknown")
    {
        m_Name = name;
        SetPosition(position);
        SetRotation(rotation);
    }

    void Object::OnUpdate()
    {
        // Move in a circular path with radius 30 units

        float radius =  1.0f;
        glm::vec3 position = GetPosition();

        // Increment the elapsed time
        elapsedTime += timeStep;

        // Calculate the new position
        float x = radius * cos(elapsedTime);
        float z = radius * sin(elapsedTime);

        SetPosition(glm::vec3(position.x + x , 30 , position.z + z));
    }

    void Object::Draw(Ref<VoxelMesh> mesh)
    {
        // Check if the position has changed
        if (prevPos != GetPosition())
        {
            // BK_TRACE("Clearing Object At : {0}, {1}, {2}", GetPosition().x, GetPosition().y, GetPosition().z);

            ClearMesh(mesh, prevPos);
            // BK_TRACE("Filling Object At : {0}, {1}, {2}", GetPosition().x, GetPosition().y, GetPosition().z);
            FillMesh(mesh, GetPosition());
            prevPos = GetPosition();
            mesh->MeshChanged = true;
        }
    }

    void Object::FillMesh(Ref<VoxelMesh> mesh, glm::vec3 position)
    {
        std::vector<std::string> m_Model = GetModel();

        std::map<glm::vec3, uint8_t, Vec3Comparator> materials;

        for (std::string line : m_Model)
        {
            std::vector<std::string> data = Boksi::ModelLoader::SplitString(line, ' ');
            BK_CORE_ASSERT(data.size() == 4, "Invalid data format");
            int x = position.x + std::stoi(data[0]);
            int y = position.y + std::stoi(data[1]);
            int z = position.z + std::stoi(data[2]);

            int m = std::stoi(data[3]);

            glm::vec3 pos = glm::vec3(x, y, z);
            uint8_t materialID = uint8_t(m);

            mesh->SetVoxel(pos, materialID);
        }
    }

    void Object::ClearMesh(Ref<VoxelMesh> mesh, glm::vec3 position)
    {
        std::vector<std::string> m_Model = GetModel();

        if (m_Model.empty())
        {
            BK_CORE_ERROR("Model is empty");
            return;
        }

        if (position == glm::vec3(-100, -100, -100))
        {
            return;
        }

        for (std::string line : m_Model)
        {
            std::vector<std::string> data = Boksi::ModelLoader::SplitString(line, ' ');
            BK_CORE_ASSERT(data.size() == 4, "Invalid data format");
            int x = position.x + std::stoi(data[0]);
            int y = position.y + std::stoi(data[1]);
            int z = position.z + std::stoi(data[2]);

            glm::vec3 pos = glm::vec3(x, y, z);

            mesh->SetVoxel(pos, EMPTY_VOXEL);
        }
    }

    void Object::CreateModel(std::vector<std::string> model)
    {
        // Take the first line as the dimension
        std::string dim = model[0].substr(2);
        std::vector<std::string> dims = Boksi::ModelLoader::SplitString(dim, ' ');
        BK_CORE_ASSERT(dims.size() == 3, "Invalid dimension format");
        int x = std::stoi(dims[0]);
        int y = std::stoi(dims[1]);
        int z = std::stoi(dims[2]);

        BK_CORE_ASSERT(x > 0 && y > 0 && z > 0, "Invalid dimensions");

        SetDimension(glm::vec3(x, y, z));

        // for (std::string line : model)
        // {
        //     BK_TRACE(line);
        // }

        BK_TRACE("Model Created");

        model.erase(model.begin());
        SetModel(model);
    }

}