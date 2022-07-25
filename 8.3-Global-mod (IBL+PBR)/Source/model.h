#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"

#include <vector>
#include <iostream>
#include <string>

class model
{

public:
	model(std::string const& path);
	void Draw(shader shader);
	void loadModel(std::string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	mesh processMesh(aiMesh* aiMesh, const aiScene* scene);
	std::vector<mesh::texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);

private:
	std::vector<mesh::texture> textures_loaded;
	std::vector<mesh> meshes;
	std::string directory;

};

#endif // !MODEL_H