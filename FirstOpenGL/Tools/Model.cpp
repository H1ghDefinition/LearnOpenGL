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

// 递归处理节点中的所有Mesh
void Model::processNode(aiNode* node, const aiScene* scene)
{
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// 接下来对它的子节点重复这一过程
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
		// 处理顶点位置、法线和纹理坐标
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
		if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
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
	// 处理索引
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	// 处理材质
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
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) // GetTextureCount检查该类型纹理的数量
	{
		aiString str;
		// 获得纹理文件
		mat->GetTexture(type, i, &str);
		// 去获取该纹理是否已经加载过
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
			// 合并文件名
			std::string filename = std::string(str.C_Str());
			filename = directory + '/' + filename;
			// 根据类型获得纹理单元
			unsigned int slot = (typeName == Diffuse) ? 0 : 1;
			texture.textureID = glUtil::TextureUtil::LoadTexture(filename.c_str(), slot);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			// 添加到已加载纹理中
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

