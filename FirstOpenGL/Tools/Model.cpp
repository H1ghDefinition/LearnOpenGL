#include "Model.h"
//#include "TextureLoader.h"
#include "glUtil.h"

Model::Model(const std::string path)
{
	loadModel(path);
}

Model::~Model()
{
	for (Mesh* mesh : meshes)
	{
		delete mesh;
	}
}
void Model::Draw(Shader& shader)
{
	for (Mesh* mesh : meshes)
	{
		mesh->Draw(shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

// �ݹ鴦��ڵ��е�����Mesh
void Model::processNode(aiNode* node, const aiScene* scene)
{
	// ����ڵ����е���������еĻ���
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// �������������ӽڵ��ظ���һ����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// ������λ�á����ߺ���������
		glm::vec3 pos;
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		vertex.position = pos;
		glm::vec3 norm;
		norm.x = mesh->mNormals[i].x;
		norm.y = mesh->mNormals[i].y;
		norm.z = mesh->mNormals[i].z;
		vertex.normal = norm;
		if (mesh->mTextureCoords[0]) // �����Ƿ����������ꣿ
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// ��������
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	// �������
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	Mesh* newMesh = new Mesh(vertices, indices, textures);

	return newMesh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) // GetTextureCount�����������������
	{
		aiString str;
		// ��������ļ�
		mat->GetTexture(type, i, &str);
		// ȥ��ȡ�������Ƿ��Ѿ����ع�
		bool skip = false;
		for (Texture& tex_loaded : textures_loaded)
		{
			if (std::strcmp(tex_loaded.path.c_str(), str.C_Str()) == 0)
			{
				textures.push_back(tex_loaded);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			// �ϲ��ļ���
			std::string filename = std::string(str.C_Str());
			filename = directory + '/' + filename;
			// �������ͻ������Ԫ
			unsigned int slot = (typeName == Diffuse) ? 0 : 1;
			texture.textureID = glUtil::TextureUtil::LoadTexture(filename.c_str(), slot);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			// ��ӵ��Ѽ���������
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

