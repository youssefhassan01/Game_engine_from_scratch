#pragma once

#include <glad/gl.h>

namespace our {

    class Texture2D {
        GLuint name = 0;//this is an opengl int 
    public:
        Texture2D() {
            //TODO: (Req 5) Complete this function
            //create a texture id
             glGenTextures(1, &name);
        };

        ~Texture2D() { 
            //TODO: (Req 5) Complete this function
            //delete the texture for resources
             glDeleteTextures(1, &name);
        }

        GLuint getOpenGLName() {
            return name;
        }

        void bind() const {
            //TODO: (Req 5) Complete this function
            //bind to opengl state machine 
             glBindTexture(GL_TEXTURE_2D, name);
        }

        static void unbind(){
            //TODO: (Req 5) Complete this function
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
    };
    
}