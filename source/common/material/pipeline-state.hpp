#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our
{
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState
    {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask, if the value is true, the corresponding channel will be written to the framebuffer, otherwise, it will not be written
        bool depthMask = true;                           // To know how to use it, check glDepthMask (true means that the depth buffer can be written to, false means that it cannot be written to)

        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const
        {
            // TODO: (Req 4) Write this function
            glColorMask(colorMask.r, colorMask.g, colorMask.b, colorMask.a); // This function specifies whether the individual color components in the framebuffer can or cannot be written
            glDepthMask(depthMask);                                          // This function specifies whether the depth buffer can or cannot be written
            if (faceCulling.enabled)                                         // if the enabled value in the faceCulling struct is true
            {
                glEnable(GL_CULL_FACE);             // enable face culling
                glCullFace(faceCulling.culledFace); // This function specifies which face will be culled
                glFrontFace(faceCulling.frontFace); // This function specifies orientation of front face (Default is GL_CCW)
            }
            else
            {
                glDisable(GL_CULL_FACE); // if the enabled value in the faceCulling struct is false, then disable face culling
            }
            if (depthTesting.enabled) // if the enabled value in the depth testing struct is true
            {
                glEnable(GL_DEPTH_TEST);            // enable depth testing
                glDepthFunc(depthTesting.function); // This draws objects based on the function specified in the depth testing struct (the depth function is what compares the depth of the new fragment with the depth of the fragment that is already stored in the depth buffer)
            }
            else
            {
                glDisable(GL_DEPTH_TEST); // if the enabled value in the depth testing struct is false, then disable depth testing
            }
            if (blending.enabled) // if the enabled value in the depth testing struct is true
            {
                glEnable(GL_BLEND);                                                                                                   // Enable Blending                                                                                                  // enable blending
                glBlendEquation(blending.equation);                                                                                   // This draws objects based on the function specified in the depth testing struct
                glBlendFunc(blending.sourceFactor, blending.destinationFactor);                                                       // This function specifies the blending factors
                glBlendColor(blending.constantColor.r, blending.constantColor.g, blending.constantColor.b, blending.constantColor.a); // This function specifies the blending color (can be used to calculate the blending factors)
            }
            else
            {
                glDisable(GL_BLEND); // if the enabled value in the depth testing struct is false, then disable depth testing
            }
        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json &data);
    };

}