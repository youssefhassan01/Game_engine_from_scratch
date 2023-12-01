#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) [Done] Create A shader program
            program = glCreateProgram();
        }
        ~ShaderProgram(){
            //TODO: (Req 1) [Done] Delete a shader program
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //TODO: (Req 1) [Done] Return the location of the uniform with the given name
            const char* c_name = name.c_str(); 
            GLuint reqUniform = glGetUniformLocation(program,c_name);
            return reqUniform;
        }

        void set(const std::string &uniform, GLfloat value) {
            //TODO: (Req 1) [Done] Send the given float value to the given uniform
            // Use getUniformLocation to find location of given name
            // Then sends it to glUniform1f to set the uniform
            GLuint reqUniform = getUniformLocation(uniform); 
            glUniform1f(reqUniform,value);
        }

        void set(const std::string &uniform, GLuint value) {
            //TODO: (Req 1) [Done] Send the given unsigned integer value to the given uniform
            // Use getUniformLocation to find location of given name
            // Then sends it to glUniform1ui to set the uniform
            GLuint reqUniform = getUniformLocation(uniform); 
            glUniform1ui(reqUniform,value);
        }

        void set(const std::string &uniform, GLint value) {
            //TODO: (Req 1) [Done] Send the given integer value to the given uniform
            // Use getUniformLocation to find location of given name
            // Then sends it to glUniform1i to set the uniform
            GLuint reqUniform = getUniformLocation(uniform); 
            glUniform1i(reqUniform,value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) [Done] Send the given 2D vector value to the given uniform
            // Use getUniformLocation to find location of given name
            // Then sends it to glUniform2f to set the uniform
            GLuint reqUniform = getUniformLocation(uniform); 
            glUniform2f(reqUniform,value.x,value.y); //since vec values are floats we use unifrom2f
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: (Req 1) [Done] Send the given 3D vector value to the given uniform
            // Use getUniformLocation to find location of given name
            // Then sends it to glUniform3f to set the uniform
            GLuint reqUniform = getUniformLocation(uniform); 
            glUniform3f(reqUniform,value.x,value.y,value.z); //since vec values are floats we use unifrom3f
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: (Req 1) [Done] Send the given 4D vector value to the given uniform
            // Use getUniformLocation to find location of given name
            // Then sends it to glUniform4f to set the uniform
            GLuint reqUniform = getUniformLocation(uniform); 
            glUniform4f(reqUniform,value.x,value.y,value.z,value.w); //since vec values are floats we use unifrom4f
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            //TODO: (Req 1) [Done] Send the given matrix 4x4 value to the given uniform
            // Use getUniformLocation to find location of given name
            // Then sends it to glUniformMatrix4fv to set the uniform
            GLuint reqUniform = getUniformLocation(uniform); 
            glUniformMatrix4fv(reqUniform,1,false,glm::value_ptr(matrix));
        }

        //TODO: (Req 1) [Done] Delete the copy constructor and assignment operator.
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        //Question: Why do we delete the copy constructor and assignment operator?
        //Answer: to prevent suprising behaviour and deep copies of constructor which might remian even if shader was removed
    };

}

#endif