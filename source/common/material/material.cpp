#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) [Done] Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 7) [Done] Write this function
        Material::setup();
        shader->set("tint",tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //TODO: (Req 7) [Done] Write this function
        TintedMaterial::setup();
        shader->set("alphaThreshold",alphaThreshold);
        glActiveTexture(GL_TEXTURE0);
        (texture != nullptr) ? texture->bind() : texture->unbind();
        (sampler != nullptr) ? sampler->bind(0) : sampler->unbind(0);
        shader->set("tex",0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void LitMaterial::setup() const{
        // Start parent (others were too complicated to start so start from Material) 
        TexturedMaterial::setup();
        glActiveTexture(GL_TEXTURE1);
        // check for diffuse texture or color and use it or unbind others that exist 
        if(albedo_map){
            albedo_map->bind();
        }
        else{
            Texture2D::unbind();
        }
        // bind sampler to it to apply options
        if(sampler){
            map_sampler->bind(1);
        }
        else{
            Sampler::unbind(1);
        }
        // call shader to set the diffuse value
        shader->set("material.diffuse_map",1);
        
        glActiveTexture(GL_TEXTURE2);
        // check for specular texture or color and use it or unbind others that exist 
        if(specular_map){
            specular_map->bind();
        }
        else{
            Texture2D::unbind();
        }
        // bind sampler to it to apply options
        if(sampler){
            map_sampler->bind(2);
        }
        else{
            Sampler::unbind(2);
        }
        // call shader to set the specular value
        shader->set("material.specular_map",2);
        
        glActiveTexture(GL_TEXTURE3);
        // check for emission texture or color and use it or unbind others that exist 
        if(emission_map){
            emission_map->bind();
        }
        else{
            Texture2D::unbind();
        }
        // bind sampler to it to apply options
        if(sampler){
            map_sampler->bind(3);
        }
        else{
            Sampler::unbind(3);
        }
        // call shader to set the emission value
        shader->set("material.emission_map",3);
        
        glActiveTexture(GL_TEXTURE4);
        // check for roughness texture or color and use it or unbind others that exist 
        if(roughness_map){
            roughness_map->bind();
        }
        else{
            Texture2D::unbind();
        }
        // bind sampler to it to apply options
        if(sampler){
            map_sampler->bind(4);
        }
        else{
            Sampler::unbind(4);
        }
        // call shader to set the roughness value
        shader->set("material.roughness_map",4);
        
        glActiveTexture(GL_TEXTURE5);
        // check for diffuse texture or color and use it or unbind others that exist 
        if(ambient_occ_map){
            ambient_occ_map->bind();
        }
        else{
            Texture2D::unbind();
        }
        // bind sampler to it to apply options
        if(sampler){
            map_sampler->bind(5);
        }
        else{
            Sampler::unbind(5);
        }
        // call shader to set the albedo value
        shader->set("material.ambient_occ_map",5);

    }

    void LitMaterial::deserialize(const nlohmann::json& data){
        TexturedMaterial::deserialize(data);

        if (!data.is_object()){
            return;
        }

        albedo_map = AssetLoader<Texture2D>::get(data.value("albedo_map","black"));
        specular_map = AssetLoader<Texture2D>::get(data.value("specular_map","black"));
        emission_map = AssetLoader<Texture2D>::get(data.value("roughness_map","black"));
        ambient_occ_map = AssetLoader<Texture2D>::get(data.value("ambient_occ_map","white"));
        roughness_map = AssetLoader<Texture2D>::get(data.value("roughness_map","black"));
    }
}