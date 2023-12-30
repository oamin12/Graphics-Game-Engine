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

    void litMaterial::setup() const
    {
        Material::setup();
        shader->set("alpha_threshold", alpha_threshold);
        
        //Albedo
        //Activating the texture unit
        glActiveTexture(GL_TEXTURE0);

        //binging the texture to the active texture unit
        if (tex_albedo)
            tex_albedo->bind();
        else
            Texture2D::unbind();
        
        //Tell OpenGL which sampler object to use
        if (sampler)
            sampler->bind(0);
        else
            Sampler::unbind(0);

        //Sending the unit number to the uniform variable
        shader->set("tex_material.albedo_map", 0);

        //Specular
        glActiveTexture(GL_TEXTURE1);

        if (tex_specular)
            tex_specular->bind();
        else
            Texture2D::unbind();
        if (sampler)
            sampler->bind(1);
        else
            Sampler::unbind(1);

        shader->set("tex_material.specular_map", 1);

        //Roughness
        glActiveTexture(GL_TEXTURE2);

        if (tex_rougness)
            tex_rougness->bind();
        else
            Texture2D::unbind();
        if (sampler)
            sampler->bind(2);
        else
            Sampler::unbind(2);

        shader->set("tex_material.roughness_map", 2);

        //Ambient Occlusion
        glActiveTexture(GL_TEXTURE3);

        if (tex_ao)
            tex_ao->bind();
        else
            Texture2D::unbind();
        if (sampler)
            sampler->bind(3);
        else
            Sampler::unbind(3);

        shader->set("tex_material.ambient_occlusion_map", 3);

        //Emission
        glActiveTexture(GL_TEXTURE4);
        
        if (tex_emission)
            tex_emission->bind();
        else
            Texture2D::unbind();
        if (sampler)
            sampler->bind(4);
        else
            Sampler::unbind(4);
        
        shader->set("tex_material.emissive_map", 4);
		
    }

    // Read the lit material data from a light object in the json file
    void litMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        alpha_threshold = data.value("alphaThreshold", 0.0f);
        // load (deserialize) texture maps from json file
        tex_albedo = AssetLoader<Texture2D>::get(data.value("albedo-texture", "black"));
        tex_specular = AssetLoader<Texture2D>::get(data.value("specular-texture", "black"));
        tex_ao = AssetLoader<Texture2D>::get(data.value("ambient-occlusion-texture", "black"));
        tex_rougness = AssetLoader<Texture2D>::get(data.value("roughness-texture", "black"));
        tex_emission = AssetLoader<Texture2D>::get(data.value("emission-texture", "black"));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", "white"));
    }
}