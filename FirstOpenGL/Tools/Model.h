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
	// �����Ѿ����ع���������ʡ��Դ
	std::vector<Texture> textures_loaded;
	std::string directory;
	// ��ָ��·������assimp��ȡģ���ļ�������
	void loadModel(const std::string path);
	// ����������������ÿ��assimp������㲢������а�����mesh���ݴ���processMesh�й���Mesh����
	void processNode(aiNode* node, const aiScene* scene);
	// ����Mesh
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	// ���ݽ�����������������
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
};