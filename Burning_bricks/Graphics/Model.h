#pragma once
#include <memory>
#include "GraphicsDefine.h"
#include "DirectXTK/Inc/Effects.h"
#include "DirectXTK/Inc/Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

_GP_BEGIN
class cModel : public DirectX::Model
{
public:
	virtual ~cModel();

	static std::unique_ptr<DirectX::Model> __cdecl CreateModel(_In_ ID3D11Device* d3dDevice, _In_reads_bytes_(dataSize) const uint8_t* meshData, _In_ size_t dataSize,
		_In_ DirectX::IEffectFactory& fxFactory, bool ccw = false, bool pmalpha = false);
	static std::unique_ptr<DirectX::Model> __cdecl CreateModel(_In_ ID3D11Device* d3dDevice, _In_z_ const wchar_t* szFileName,
		_In_ DirectX::IEffectFactory& fxFactory, bool ccw = false, bool pmalpha = false);
private:
	static DirectX::ModelMesh::Collection ProcessNode(aiNode* node, aiScene const* scene, _In_ ID3D11Device* d3dDevice, _In_ DirectX::IEffectFactory& fxFactory, bool ccw, bool pmalpha);
};
_GP_END