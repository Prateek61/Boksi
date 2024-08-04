#include "bkpch.h"
#include "World.h"

namespace Boksi
{

    class ModelLoader
    {
    public:
        ModelLoader() = default;
        ~ModelLoader() = default;

        static void LoadModel(const std::string path , Ref<World> world , glm::uvec3 pos = {0, 0, 0} , int scale = 1);
            
        static std::vector<std::string> SplitString(const std::string &s, char delimiter)
        {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(s);
            while (std::getline(tokenStream, token, delimiter))
            {
                tokens.push_back(token);
            }
            return tokens;
        }
    };

}