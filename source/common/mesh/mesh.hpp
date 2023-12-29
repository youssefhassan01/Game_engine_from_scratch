#pragma once

#include <glad/gl.h>
#include "vertex.hpp"
//note to self : binding and un binding is done to add the array in the opengl state machine 
//or remove it 
//deleting it removes the array releases it enitrly from the gpu
namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        //the VBO (vertex buffer object) contains the data of the vertex 
        //EBO elment buffer object represents the drawing order of the vertices 
        //VAO connects both the elemt and vertex as it has the settings of it 
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
            //TODO: (Req 2) Write this function     [DONE]
            elementCount = (int)elements.size();

            //define a vertex array and bind the array to the gpu 
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
        //define a vertex buffer and bind the array to the gpu 
    //assign the type/size/data/and waht we will do with it in this case draw
            glGenBuffers(1, &VBO);      
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            // Set the vertex attribute pointers
            //here we are defining the struct elements of the struct of the vertex 
            //postion /color /text cordainate /noraml 
            //the argumnts are: attribute type / number of arrgumnets /data type /normalise or not/ size in the struct/ off set in the struct

            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
 

            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));

            //this is the noraml attribute of the mesh (where the surface is pointing) 3 is for the x,y,z
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));


            // Create the element buffer
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);


            //unbinding buffers and vertex array
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function    [DONE]

            
            glBindVertexArray(VAO);
            
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
            
            glBindVertexArray(0);

        }

        ~Mesh(){
            //TODO: (Req 2) Write this function   [DONE]
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}