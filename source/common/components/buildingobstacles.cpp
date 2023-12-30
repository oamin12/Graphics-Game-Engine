#include "buildingobstacles.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    void BuildingObstaclesComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
    }
}