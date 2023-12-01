#pragma once

#include <glad/gl.h>

namespace our
{

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D
    {
        // The OpenGL object name of this texture
        GLuint name = 0;

    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name"
        Texture2D()
        {
            // TODO: (Req 5) Complete this function
            glGenTextures(1, &name); // Generate a texture object name, (1st attirbute: number of textures, 2nd attribute: pointer to the variable that will store the texture object name)
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D()
        {
            glDeleteTextures(1, &name); // Delete the texture object name, (1st attirbute: number of textures, 2nd attribute: pointer to the variable that stores the texture object name
            // TODO: (Req 5) Complete this function
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName()
        {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const
        {
            glBindTexture(GL_TEXTURE_2D, name); // Bind the texture object name to GL_TEXTURE_2D
            // TODO: (Req 5) Complete this function
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind()
        {
            glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture
            // TODO: (Req 5) Complete this function
        }

        Texture2D(const Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
    };

}