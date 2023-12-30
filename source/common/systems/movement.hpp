#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../components/camera.hpp"
#include "../components/obstacles.hpp"
#include "../components/buildingobstacles.hpp"
#include "../components/treeobstacles.hpp"
#include <iostream>
namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class MovementSystem
    {
    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        bool checkCollision(World *world, glm::vec3 position)
        {
            for (auto entity : world->getEntities())
            {
                ObstaclesComponent *Obstacle = entity->getComponent<ObstaclesComponent>();
                BuildingObstaclesComponent *BuildingObstacle = entity->getComponent<BuildingObstaclesComponent>();
                TreeObstaclesComponent *TreeObstacle = entity->getComponent<TreeObstaclesComponent>();
                // printf("In collision before obstacles check\n");
                if (BuildingObstacle)
                {
                    float obstacle_x = entity->localTransform.position.x;
                    float obstacle_z = entity->localTransform.position.z;
                    if (position.x < obstacle_x + 1.3 && position.x > obstacle_x - 1.3 && position.z < obstacle_z + 9 && position.z > obstacle_z - 9) // building obstacles collision check
                        return true;
                }
                else if (TreeObstacle)
                {
                    float obstacle_x = entity->localTransform.position.x;
                    float obstacle_z = entity->localTransform.position.z;
                    if (position.x < obstacle_x + 1.5 && position.x > obstacle_x - 1.5 && position.z < obstacle_z + 1 && position.z > obstacle_z - 1) // building obstacles collision check
                        return true;
                }
                else if (Obstacle)
                {
                    float obstacle_x = entity->localTransform.position.x;
                    float obstacle_z = entity->localTransform.position.z;
                    if (position.x < obstacle_x + 1 && position.x > obstacle_x - 1 && position.z < obstacle_z + 1.5 && position.z > obstacle_z - 1.5) // building obstacles collision check
                        return true;
                }
            }
            return false;
        }
        void update(World *world, float deltaTime)
        {
            // glm::vec3 caroffset = glm::vec3(0.0f, 1.8f, 10.0f);
            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                glm::vec3 caroffset = glm::vec3(0.0f, 1.8f, 4.0f);
                // Get the movement component if it exists
                MovementComponent *movement = entity->getComponent<MovementComponent>();
                CameraComponent *camera = entity->getComponent<CameraComponent>();

                // If the movement component exists
                if (camera)
                {
                    // if (entity->localTransform.position.y < -15.0f) {
                    //     // return to original coordinares (respawn)
                    //     entity->localTransform.position.x = 0.0f + 8 * glm::sin(entity->localTransform.rotation.y);
                    //     entity->localTransform.position.y = 0.0f;
                    //     entity->localTransform.position.z = 2.0f + 8 * glm::cos(entity->localTransform.rotation.y);
                    //     // this time, we add the correction instead of removing it, as this is relative to the camera
                    //     // (k2nena bn3ml el 3aks)
                    // }
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    // entity->localTransform.position += deltaTime * movement->linearVelocity;
                    // entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                    // printf("%s\n", movement->getID().c_str());
                    bool collided = checkCollision(world, entity->localTransform.position - caroffset);
                    if (collided)
                    {
                        printf("Collision detected\n");
                    }
                }
            }
        }
    };

}
