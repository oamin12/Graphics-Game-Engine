#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <irrKlang.h>
using namespace irrklang;
// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    ISoundEngine *mp3Teryaki = nullptr;

    void onInitialize() override
    {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
        // songsssssssss TOKYOOOOOOOOO
        mp3Teryaki = createIrrKlangDevice();
        mp3Teryaki->setSoundVolume(0.1f); 
        mp3Teryaki->play2D("assets/music/tokyo.mp3", false, false, true);
    }

    void onDraw(double deltaTime) override
    {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        // Get a reference to the keyboard object
        auto &keyboard = getApp()->getKeyboard();

        // check if shift is pressed
        if (keyboard.isPressed(GLFW_KEY_LEFT_SHIFT))
        {
            // If the shift key is pressed we want to activate motion blur
            renderer.changeIsShiftPressed(true);
        }
        else
        {
            // If the shift key is not pressed we want to deactivate motion blur
            renderer.changeIsShiftPressed(false);
        }

        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        if (keyboard.justPressed(GLFW_KEY_ESCAPE))
        {
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
        if (world.getCollided()) // if player died --> go to state  Loose state
        {
            getApp()->changeState("lose");
            world.setCollided(false); // reset the Collided bool to false
        }
        if (world.getFinished_Race()) // if player wins --> go to state  Win state
        {
            getApp()->changeState("win");
            world.setFinished_Race(false); // reset the Finished_race bool to false
        }
    }

    void onDestroy() override
    {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
        delete mp3Teryaki;
    }
};