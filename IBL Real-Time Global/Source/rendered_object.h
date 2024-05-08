#pragma once

#include <string>
#include <vector>

#include "stb_image.h"
#include "shader.h"
#include "rendered_structures.h"

class rendered_object
{
public:
	rendered_object();
	rendered_object(std::string);

	void bind_shader(shader*);
	void bind_light_source(int, light_source);
	void bind_material(material);
	void bind_matrices(glm::mat4, glm::mat4, glm::mat4);
	void bind_matrices(glm::mat4, glm::mat4, glm::mat4, glm::vec3);

	unsigned int load_texture(std::string path);
	void load_all_textures(std::string path);
	void active_textures();

private:
	std::string rendered_type;
	shader* connected_shader = NULL;
	std::vector<unsigned> textures;
};

