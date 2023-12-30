#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace our
{
    // The light type enum 
    // 0 = directional light
    // 1 = point light
    // 2 = spot light
    enum class LightType
    {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class LightComponent : public Component
    {
    public:
        LightType LightType;   // The type of the light
        glm::vec3 color;       // color of light
        glm::vec2 coneAngles;  // angles of spot light cone
        glm::vec3 attenuation; // attenuation of light (constant, linear, quadratic)

        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json& data) override;

        // get the color of the light
        glm::vec3 getLightColor();
    };

}