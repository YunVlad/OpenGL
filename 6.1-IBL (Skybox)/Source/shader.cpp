#include "shader.h"

shader::shader(std::string pathVertex, std::string pathFragment, bool isDebug)
{
	std::ifstream fileVertex;
	std::ifstream fileFragment;

	fileVertex.open(pathVertex);
	fileFragment.open(pathFragment);

	std::string StringCodeVertex, StringCodeFragment, temp;
	std::stringstream vShaderStream, fShaderStream;

	vShaderStream << fileVertex.rdbuf();
	fShaderStream << fileFragment.rdbuf();

	StringCodeVertex = vShaderStream.str();
	StringCodeFragment = fShaderStream.str();

	const char* CharVertexCode = StringCodeVertex.c_str();
	const char* CharFragmentCode = StringCodeFragment.c_str();

	fileVertex.close();
	fileFragment.close();

	unsigned int vertexShader, fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &CharVertexCode, NULL);
	glCompileShader(vertexShader);

	if (isDebug)
	{
		checkCompileErrors(vertexShader, "VERTEX", pathVertex);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &CharFragmentCode, NULL);
	glCompileShader(fragmentShader);

	if (isDebug)
	{
		checkCompileErrors(vertexShader, "VERTEX", pathFragment);
	}

	pointer = glCreateProgram();
	glAttachShader(pointer, vertexShader);
	glAttachShader(pointer, fragmentShader);
	glLinkProgram(pointer);

	if (isDebug)
	{
		std::string prog = "";
		checkCompileErrors(vertexShader, "PROGRAM", prog);
	}
}

shader::shader(std::string pathVertex, std::string pathFragment, std::string pathGeom)
{
	std::ifstream fileVertex;
	std::ifstream fileFragment;
	std::ifstream fileGeom;

	fileVertex.open(pathVertex);
	fileFragment.open(pathFragment);
	fileGeom.open(pathGeom);

	std::string StringCodeVertex, StringCodeFragment, StringCodeGeom, temp;
	std::stringstream vShaderStream, fShaderStream, gShaderStream;

	vShaderStream << fileVertex.rdbuf();
	fShaderStream << fileFragment.rdbuf();
	gShaderStream << fileGeom.rdbuf();

	StringCodeVertex = vShaderStream.str();
	StringCodeFragment = fShaderStream.str();
	StringCodeGeom = gShaderStream.str();

	const char* CharVertexCode = StringCodeVertex.c_str();
	const char* CharFragmentCode = StringCodeFragment.c_str();
	const char* CharGeomCode = StringCodeGeom.c_str();

	fileVertex.close();
	fileFragment.close();
	fileGeom.close();

	unsigned int vertexShader, fragmentShader, geomShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &CharVertexCode, NULL);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &CharFragmentCode, NULL);
	glCompileShader(fragmentShader);

	geomShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geomShader, 1, &CharGeomCode, NULL);
	glCompileShader(geomShader);

	pointer = glCreateProgram();
	glAttachShader(pointer, vertexShader);
	glAttachShader(pointer, fragmentShader);
	glAttachShader(pointer, geomShader);
	glLinkProgram(pointer);
}

void shader::UseShaderProgramm()
{
	glUseProgram(pointer);
}

void shader::IntUniform(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(pointer, name.c_str()), value);
}

void shader::FloatUniform(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(pointer, name.c_str()), value);
}

void shader::vec3Uniform(const std::string& name,
						 float value1, float value2, float value3) const
{
	glUniform3f(glGetUniformLocation(pointer, name.c_str()), value1, value2, value3);
}

void shader::vec3Uniform(const std::string& name, glm::vec3 vec) const
{
	glUniform3f(glGetUniformLocation(pointer, name.c_str()), vec.x, vec.y, vec.z);
}

void shader::vec4Uniform(const std::string& name, 
						 float value1, float value2, float value3, float value4) const
{
	glUniform4f(glGetUniformLocation(pointer, name.c_str()), value1, value2, value3, value4);
}

void shader::mat4Uniform(const std::string& name, glm::mat4 mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(pointer, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void shader::checkCompileErrors(GLuint shader, std::string type, std::string path)
{
	GLint success;
	GLchar infoLog[1024];

	if (type != "PROGRAM")
	{
		std::cout << "START_OF_COMPILATION_AND_LINKING: " << path << std::endl;

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR. TYPE: " << type << std::endl << infoLog << std::endl;
		}
		else
		{
			std::cout << "THE_SHADERS_HAVE_BEEN_COMPILED_SUCCESSFULLY" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "PROGRAM_LINKING_ERROR. TYPE: " << type << "\n" << infoLog << std::endl;
		}
		else
		{
			std::cout << "THE_PROGRAM_WAS_SUCCESSFULLY_LINKED" << std::endl << std::endl;
		}
	}
}