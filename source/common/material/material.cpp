#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) Write this function
        pipelineState.setup();//setup the pipeline state
        shader->use();//set the shader to be used

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
        //TODO: (Req 7) Write this function
        Material::setup();//call the setup of its parent
        shader->set("tint", tint);//set the "tint" uniform to the value in the member variable tint
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
        //TODO: (Req 7) Write this function
        TintedMaterial::setup();

        shader->set("alphaThreshold", alphaThreshold);//set alphaThreshold uniform to the value in the member variable alphaThreshold
        // the following line activates the first texture unit. Texture units are slots to which textures can be bound for use in the shader program.
        glActiveTexture(GL_TEXTURE0);// ta2reeban can be removed as it is the default texture unit

        //binding the object means to make it the current object that will be used in the next operations
        if (texture)//if texture is not null then bind it
            texture->bind();

        if (sampler)
            sampler->bind(0); //bind the sampler to the first texture unit (0) 

            
        //bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
        shader->set("tex", 0);

    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}