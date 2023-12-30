#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../components/camera.hpp"
#include "../components/obstacles.hpp"
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
                // printf("In collision before obstacles check\n");
                if (Obstacle)
                {
                    float obstacle_x = entity->localTransform.position.x;
                    float obstacle_z = entity->localTransform.position.z;
                    if (sqrt((position.x - obstacle_x) * (position.x - obstacle_x) + (position.z - obstacle_z) * (position.z - obstacle_z)) < 5)
                    {
                        printf("Colided fucker moment with %s\n", Obstacle->getID().c_str());
                        return true;
                    }
                }
            }
            return false;
        }
        void update(World *world, float deltaTime)
        {
            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                // Get the movement component if it exists
                MovementComponent *movement = entity->getComponent<MovementComponent>();
                // If the movement component exists
                if (movement)
                {
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                    // printf("%s\n", movement->getID().c_str());
                    bool collided = checkCollision(world, entity->localTransform.position);
                    // if (collided)
                    // {
                    //     printf("Collision detected\n");
                    // }
                }
            }
        }
    };

}
