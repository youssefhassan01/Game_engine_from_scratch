#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

our::Texture2D *our::texture_utils::empty(GLenum format, glm::ivec2 size)
{
    our::Texture2D *texture = new our::Texture2D();
    // TODO: (Req 11) Finish this function to create an empty texture with the given size and format

    texture->bind();
    //GL_UNPACK_ALIGNMENT: this determines the unpack alignemnt of the texture pack 
    //4 sets the start of the alignment location in memory
    //as most rows are 32 bits we use 4 by defualt as in 4 bytes 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    //the target,mip level , texture formate ,size ,border ,pixel formate ,data typen of teh pixel ,intial pointer to the data is null 
    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, nullptr);


    texture->unbind();
    return texture;
}

our::Texture2D *our::texture_utils::loadImage(const std::string &filename, bool generate_mipmap)
{
    glm::ivec2 size;
    int channels;
//we draw form the top left so we have to flip vertically to start from the bottom left 
    stbi_set_flip_vertically_on_load(true);

    unsigned char *pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if (pixels == nullptr)
    {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    our::Texture2D *texture = new our::Texture2D();

    // TODO: (Req 5) Finish this function to fill the texture with the data found in "pixels"
    texture->bind();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    if (generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels); 
    texture->unbind();

    return texture;
}