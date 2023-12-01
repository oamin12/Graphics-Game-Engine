#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        //TODO: (Req 8) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to the world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M

        glm::vec4 eye = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //point
        glm::vec4 center = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f); //point
        glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f); //vector

        glm::vec3 eyeWorld = M * eye;
        glm::vec3 centerWorld = M * center;
        glm::vec3 upWorld = M * up;

        return glm::lookAt(eyeWorld, centerWorld, upWorld);
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //TODO: (Req 8) Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective
        float aspectRatio = static_cast<float>(viewportSize.x) / static_cast<float>(viewportSize.y);

        float bottom = -orthoHeight / 2;
        float top = orthoHeight / 2;
        float left = bottom * aspectRatio;
        float right = top * aspectRatio;

        if (cameraType == CameraType::ORTHOGRAPHIC) {
            return glm::ortho(left, right, bottom, top, near, far);
        }
        
        return glm::perspective(fovY, aspectRatio, near, far);
    }
}