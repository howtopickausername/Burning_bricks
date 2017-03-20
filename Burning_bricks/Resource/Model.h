#pragma once
#include <string>
#include <memory>
#include <vector>
#include "assimp/scene.h"
#include "Mesh.h"

class cModel
{
public:
	cModel(ID3D11Device& device);
	~cModel();
	void LoadModel(std::string modelPath);
	void Draw(ID3D11DeviceContext& context, Gp::cEffect& effect);

private:
	void ProcessNode(aiNode* node, aiScene const* scene);
	cMesh ProcessMesh(aiMesh* mesh, aiScene const* scene);
	std::vector<cMesh> m_Meshes;
	ID3D11Device& m_Device;
};
typedef std::shared_ptr<cModel> pModel;