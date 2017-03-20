#include "stdafx.h"
#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "ResourceExcepthion.h"

cModel::~cModel()
{

}

cModel::cModel(ID3D11Device& device)
	:m_Device(device)
{

}

void cModel::LoadModel(std::string modelPath)
{
	Assimp::Importer importer;
	aiScene const* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		throw cResourceExcep("LoadModel failed:" + modelPath);
	}
	this->ProcessNode(scene->mRootNode, scene);
}

void cModel::Draw(ID3D11DeviceContext& context, Gp::cEffect& effect)
{
	for (auto& mesh : m_Meshes)
	{
		mesh.Draw(context, effect);
	}
}

void cModel::ProcessNode(aiNode* node, aiScene const* scene)
{
	for (int i = 0; i < node->mNumMeshes ; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(this->ProcessMesh(mesh, scene));
	}
	for (int i = 0; i < node->mNumChildren ; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

cMesh cModel::ProcessMesh(aiMesh* mesh, aiScene const* scene)
{
	std::vector<cVertex> vertices;
	std::vector<short> indeices;
	
	for (int i = 0; i < mesh->mNumVertices ; i++)
	{
		cVertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;
		if (mesh->mTextureCoords[0]){
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		vertices.push_back(vertex);
	}

	for (int i = 0; i < mesh->mNumFaces ; i++)
	{
		for (int j = 0; j < mesh->mFaces[i].mNumIndices ; j++)
		{
			indeices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
	return cMesh(m_Device, std::move(vertices), std::move(indeices));
}
