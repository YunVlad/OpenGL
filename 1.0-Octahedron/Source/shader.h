#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class shader
{

public:

	unsigned int pointer;

	shader(std::string, std::string);

	void FloatUniform(const std::string&, float) const;
	void vec4Uniform(const std::string&, float, float, float, float) const;
	void mat4Uniform(const std::string&, glm::mat4) const;

	void UseShaderProgramm();

};

#endif // !SHADER_H

