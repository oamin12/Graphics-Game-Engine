#include "light.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include "../deserialize-utils.hpp"

namespace our
{
    // Reads light parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        std::string lightTypeStr = data.value("lightType", "directional"); // default light type is directional
        if (lightTypeStr == "directional")
        {
            LightType = LightType::DIRECTIONAL;
        }
        else if (lightTypeStr == "point")
        {
            LightType = LightType::POINT;
        }
        else
        {
            LightType = LightType::SPOT;
        }
        color = data.value("color", glm::vec3(1.0f, 1.0f, 1.0f));
        coneAngles = data.value("coneAngles", glm::vec2(glm::radians(30.0f), glm::radians(45.0f)));
        attenuation = data.value("attenuation", glm::vec3(0.0f, 0.0f, 1.0f));
    }

    glm::vec3 LightComponent::getLightColor()
    {
        return color;
    }
}