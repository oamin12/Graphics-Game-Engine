#include "treeobstacles.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    void TreeObstaclesComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
    }
}