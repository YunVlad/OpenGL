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

	shader(std::string pathVertex, std::string pathFragment);

	void FloatUniform(const std::string& name, float value) const;

	void vec3Uniform(const std::string& name,
					 float value1, float value2, float value3) const;

	void vec3Uniform(const std::string& name, glm::vec3 vec) const;

	void vec4Uniform(const std::string& name,
					 float value1, float value2, float value3, float value4) const;

	void mat4Uniform(const std::string& name, glm::mat4 mat) const;

	void UseShaderProgramm();

};

#endif // !SHADER_H

