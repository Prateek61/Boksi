#include "bkpch.h"
#include "ModelLoader.h"
#include "World.h"


namespace Boksi
{
    
    void ModelLoader::LoadModel(const std::string path , Ref<World> world , glm::uvec3 pos , int scale)
    {
        //load file from path in read mode

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            BK_CORE_ERROR("Could not open file: {0}", path);
            return;
        }

        //read the file
        std::string line;
        while (std::getline(file, line))
        {
            // first line is the dimension
            if (line[0] == 'D')
            {
                std::string dim = line.substr(2);
                std::vector<std::string> dims = SplitString(dim, ' ');
                BK_CORE_ASSERT(dims.size() == 3, "Invalid dimension format");
                int x = std::stoi(dims[0]);
                int y = std::stoi(dims[1]);
                int z = std::stoi(dims[2]);
                
                BK_CORE_ASSERT(x > 0 && y > 0 && z > 0, "Invalid dimensions");
            }

            else
            {
                std::vector<std::string> data = SplitString(line, ' ');
                BK_CORE_ASSERT(data.size() == 6, "Invalid data format");
                int x = pos.x + std::stoi(data[0]) * scale;
                int y = pos.y + std::stoi(data[1]) * scale;
                int z = pos.z + std::stoi(data[2]) * scale;

                int r = std::stoi(data[3]);
                int g = std::stoi(data[4]);
                int b = std::stoi(data[5]);
                
                BK_CORE_ASSERT(x >= 0 && x < world->GetSize().x, "Invalid x coordinate");
                BK_CORE_ASSERT(y >= 0 && y < world->GetSize().y, "Invalid y coordinate");
                BK_CORE_ASSERT(z >= 0 && z < world->GetSize().z, "Invalid z coordinate");

                glm::uvec3 pos = {x, y, z};
                Voxel voxel = 2;
                world->SetVoxel(pos, voxel);

            }

        }
        
        
    }

} // namespace Boksi
