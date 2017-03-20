#pragma once
#include <string>
#include <map>
#include <vector>
#include "DirectXTK/Inc/Effects.h"
#include "GraphicsDefine.h"
#include "DirectXTK/Inc/Model.h"

_GP_BEGIN;
class cModelRes
{
public:
	void Init(ID3D11Device& device, DirectX::EffectFactory& effFactory, std::vector<std::wstring> resPaths);
	DirectX::Model* GetModel(std::wstring name);

protected:
	std::map<std::wstring, std::unique_ptr<DirectX::Model>> m_Models;
};
_GP_END;