#include "rendered_object.h"

rendered_object::rendered_object(){}

rendered_object::rendered_object(std::string str)
{
    rendered_type = str;
}


void rendered_object::bind_shader(shader* shader)
{
	connected_shader = shader;
}

void rendered_object::bind_light_source(int number, light_source light)
{
    connected_shader->UseShaderProgramm();
    connected_shader->vec3Uniform("lights[" + std::to_string(number) + "].pos", light.pos);
    connected_shader->vec3Uniform("lights[" + std::to_string(number) + "].color", light.color);
    connected_shader->vec3Uniform("lights[" + std::to_string(number) + "].atten", light.atten.x, light.atten.y, light.atten.z);
    connected_shader->FloatUniform("lights[" + std::to_string(number) + "].diff", light.diff);
    connected_shader->FloatUniform("lights[" + std::to_string(number) + "].spec", light.spec);
}

void rendered_object::bind_material(material mater)
{
    connected_shader->UseShaderProgramm();
    connected_shader->vec3Uniform("material.diffuse", mater.diffuse);
    connected_shader->vec3Uniform("material.specular", mater.specular);
    connected_shader->FloatUniform("material.shininess", mater.shininess);
}

void rendered_object::bind_matrices(glm::mat4 model, glm::mat4 view, glm::mat4 proj)
{
    connected_shader->mat4Uniform("model", model);
    connected_shader->mat4Uniform("view", view);
    connected_shader->mat4Uniform("proj", proj);
}

void rendered_object::bind_matrices(glm::mat4 model, glm::mat4 view, glm::mat4 proj, glm::vec3 viewPos)
{
    connected_shader->mat4Uniform("model", model);
    connected_shader->mat4Uniform("view", view);
    connected_shader->mat4Uniform("proj", proj);
    connected_shader->vec3Uniform("viewPos", viewPos);
}


unsigned int rendered_object::load_texture(std::string path)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

void rendered_object::load_all_textures(std::string path)
{
    textures.push_back(load_texture(path + "/albedo.jpg"));
    textures.push_back(load_texture(path + "/normal.jpg"));
    textures.push_back(load_texture(path + "/metallic.jpg"));
    textures.push_back(load_texture(path + "/roughness.jpg"));
}

void rendered_object::active_textures()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[0]);;
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
}