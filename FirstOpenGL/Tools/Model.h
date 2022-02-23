#pragma once
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model
{
public:
	Model(const std::string path);
	~Model();
	void Draw(Shader& shader);
private:
	std::vector<Mesh*> meshes;
	// 保存已经加载过的纹理，节省资源
	std::vector<Texture> textures_loaded;
	std::string directory;
	// 从指定路径中由assimp读取模型文件并解析
	void loadModel(const std::string path);
	// 处理解析结果，遍历每个assimp场景结点并将结点中包含的mesh数据传到processMesh中构造Mesh对象
	void processNode(aiNode* node, const aiScene* scene);
	// 构造Mesh
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	// 根据解析结果生成纹理对象
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
};