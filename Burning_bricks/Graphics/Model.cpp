#include "stdafx.h"
#include "Model.h"
#include <algorithm>
#include "GraphicsException.h"
#include "DirectXTK/Src/BinaryReader.h"
#include "DirectXTK/Inc/SimpleMath.h"
#include "DirectXTK/Src/PlatformHelpers.h"
#include "Locator.h"

Gp::cModel::~cModel()
{

}

std::unique_ptr<DirectX::Model> __cdecl Gp::cModel::CreateModel(_In_ ID3D11Device* d3dDevice, _In_reads_bytes_(dataSize) const uint8_t* meshData, _In_ size_t dataSize, _In_ DirectX::IEffectFactory& fxFactory, bool ccw /*= false*/, bool pmalpha /*= false*/)
{
	if (!d3dDevice || !meshData)
		throw Gp::Exception("Device and meshData cannot be null");
	Assimp::Importer importer;
	aiScene const* scene = importer.ReadFileFromMemory(meshData, dataSize, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw Gp::Exception("LoadModel failed");
	}
	auto meshes = ProcessNode(scene->mRootNode, scene, d3dDevice, fxFactory, ccw, pmalpha);
	auto model = std::make_unique<DirectX::Model>();
	model->meshes.insert(std::end(model->meshes), std::begin(meshes), std::end(meshes));
	return model;
}

DirectX::ModelMesh::Collection Gp::cModel::ProcessNode(aiNode* node, aiScene const* scene, _In_ ID3D11Device* d3dDevice, _In_ DirectX::IEffectFactory& fxFactory, bool ccw, bool pmalpha)
{
	DirectX::ModelMesh::Collection meshes;
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];

 		auto mesh = std::make_shared<DirectX::ModelMesh>();
 		mesh->ccw = ccw;
 		mesh->pmalpha = pmalpha;
		wchar_t meshName[256];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, aimesh->mName.C_Str(), -1, meshName, 256);
		mesh->name = meshName;

		class cVertex {
		public:
			DirectX::XMFLOAT3 Position;
			DirectX::XMFLOAT3 Normal;
			DirectX::XMFLOAT2 TexCoords;
		};
		std::vector<cVertex> vertices;
		std::vector<short> indices;
		for (int i = 0; i < aimesh->mNumVertices; i++) {
			cVertex vertex;
			vertex.Position.x = aimesh->mVertices[i].x;
			vertex.Position.y = aimesh->mVertices[i].y;
			vertex.Position.z = aimesh->mVertices[i].z;
			vertex.Normal.x = aimesh->mNormals[i].x;
			vertex.Normal.y = aimesh->mNormals[i].y;
			vertex.Normal.z = aimesh->mNormals[i].z;
			if (aimesh->mTextureCoords[0]) {
				vertex.TexCoords.x = aimesh->mTextureCoords[0][i].x;
				vertex.TexCoords.y = aimesh->mTextureCoords[0][i].y;
			}
			vertices.push_back(vertex);
		}
		for (int i = 0; i < aimesh->mNumFaces; i++) {
			for (int j = 0; j < aimesh->mFaces[i].mNumIndices; j++) {
				indices.push_back(aimesh->mFaces[i].mIndices[j]);
			}
		}
		ID3D11Buffer* verBuf;
		ID3D11Buffer* iBuf;
		D3D11_BUFFER_DESC descBuf;
		ZeroMemory(&descBuf, sizeof(D3D11_BUFFER_DESC));
		D3D11_SUBRESOURCE_DATA resData;
		ZeroMemory(&resData, sizeof(resData));

		descBuf.Usage = D3D11_USAGE_DYNAMIC;
		descBuf.ByteWidth = sizeof(cVertex) * vertices.size();
		descBuf.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		descBuf.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		descBuf.MiscFlags = 0;
		resData.pSysMem = &vertices[0];
		HRESULT hr = d3dDevice->CreateBuffer(&descBuf, &resData, &verBuf);
		if (FAILED(hr)) {
			throw Gp::Exception("Mesh CreateVertexBuffer failed." + std::to_string(hr));
		}

		descBuf.ByteWidth = sizeof(short) * indices.size();
		descBuf.BindFlags = D3D11_BIND_INDEX_BUFFER;
		resData.pSysMem = &indices[0];
		hr = d3dDevice->CreateBuffer(&descBuf, &resData, &iBuf);
		if (FAILED(hr)) {
			throw Gp::Exception("Mesh CreateIndexBuffer failed." + std::to_string(hr));
		}

		DirectX::EffectFactory::EffectInfo info;
		info.alpha = 1;
		info.name = meshName;
		info.perVertexColor = false;
		info.enableSkinning = false;
		info.enableDualTexture = false;
		info.enableNormalMaps = false;
		info.ambientColor = DirectX::SimpleMath::Vector3::One;
		info.diffuseColor = DirectX::SimpleMath::Vector3::One;
		info.emissiveColor = DirectX::SimpleMath::Vector3::One;
		info.diffuseTexture = nullptr;
		info.specularTexture = nullptr;
		info.normalTexture = nullptr;
		auto effect = fxFactory.CreateEffect(info, nullptr);

		std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC> > elements = std::make_shared<std::vector<D3D11_INPUT_ELEMENT_DESC>>(
			std::vector<D3D11_INPUT_ELEMENT_DESC>{
			{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		});
		void const* shaderByteCode;
		size_t byteCodeLength;
		effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
		ID3D11InputLayout* pInputLayout;
		DirectX::ThrowIfFailed(
			d3dDevice->CreateInputLayout((*elements).data(),
				static_cast<UINT>((*elements).size()),
				shaderByteCode, byteCodeLength,
				&pInputLayout)
			);

		auto part = new DirectX::ModelMeshPart();
		part->isAlpha = false;
		part->indexCount = static_cast<uint32_t>(indices.size());
		part->startIndex = static_cast<uint32_t>(0);
		part->vertexOffset = static_cast<uint32_t>(0);
		part->vertexStride = static_cast<uint32_t>(sizeof(cVertex));
		part->indexFormat = DXGI_FORMAT_R16_UINT;
		part->primitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		part->inputLayout = pInputLayout;
		part->indexBuffer = iBuf;
		part->vertexBuffer = verBuf;
		part->effect = effect;
		part->vbDecl = elements;

		mesh->meshParts.emplace_back(part);
		meshes.emplace_back(mesh);
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		DirectX::ModelMesh::Collection childMeshes = ProcessNode(node->mChildren[i], scene, d3dDevice, fxFactory, ccw, pmalpha);
		meshes.insert(std::end(meshes), std::begin(childMeshes), std::end(childMeshes));
	}
	return std::move(meshes);
}

std::unique_ptr<DirectX::Model> __cdecl Gp::cModel::CreateModel(_In_ ID3D11Device* d3dDevice, _In_z_ const wchar_t* szFileName, _In_ DirectX::IEffectFactory& fxFactory, bool ccw /*= false*/, bool pmalpha /*= false*/)
{
	size_t dataSize = 0;
	std::unique_ptr<uint8_t[]> data;
	HRESULT hr = DirectX::BinaryReader::ReadEntireFile(szFileName, data, &dataSize);
	if (FAILED(hr)) {
		cLocator::Log() << ("CreateModel failed (%08X) loading '%ls'\n", hr, szFileName);
		throw Gp::Exception("CreateModel");
	}

	auto model = CreateModel(d3dDevice, data.get(), dataSize, fxFactory, ccw, pmalpha);

	model->name = szFileName;

	return model;
}
