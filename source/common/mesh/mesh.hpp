#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            elementCount = (GLsizei)elements.size();

            //create a vertex Array object
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            //create vertex buffer
            glGenBuffers(1, &VBO); //create one buffer
            glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind the buffer to the GL_ARRAY_BUFFER target
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW); //copy the data from vertices to the buffer

            //create element buffer
            glGenBuffers(1, &EBO); //create one buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the buffer to the GL_ELEMENT_ARRAY_BUFFER target
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW); //copy the data from elements to the buffer

            //set the vertex attribute pointers
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);

            // in the color attribute, we use the normalized version of the Color data type
            // also the type of the color attribute is GL_UNSIGNED_BYTE instead of GL_FLOAT since we want to store the color in bytes instead of floats
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);


            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);


            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);


            //unbind the buffers
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            
        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function

            //bind the vertex array object
            glBindVertexArray(VAO); // we must bind the VAO before drawing so that OpenGL knows how to read the vertex data from the buffers

            //draw the mesh
            // hena rasamna el primitive type ely howa GL_TRIANGLES!!
            // el element count howa ely howa el size of the element buffer
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0);

            //unbind the vertex array object
            glBindVertexArray(0);

        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 2) Write this function
            //delete the buffers and the vertex array object
            // the first argument of glDeleteBuffers and glDeleteVertexArrays is the number of buffers/vertex array objects to delete
            
            //we check if the buffers and the vertex array object are not zero before deleting them to avoid a crash if the destructor is called twice by mistake (double free)
            if (VAO != 0)
                glDeleteBuffers(1, &VBO);
            if (VBO != 0)
                glDeleteBuffers(1, &EBO);
            if (EBO != 0)
                glDeleteVertexArrays(1, &VAO);

        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}