#pragma once
#include <vector>
#include <D3D11.h>
#include <DirectXMath.h>
#include "Graphics/Effect.h"

class cVertex {
public:
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 TexCoords;
};

class cMesh
{
public:
	cMesh(ID3D11Device& device, std::vector<cVertex> const& v, std::vector<short> const& i);
	~cMesh();

	std::vector<cVertex> vertices;
	std::vector<short> indices;
	void Draw(ID3D11DeviceContext& context, Gp::cEffect& effect);
private:
	void InitBuf(ID3D11Device& device);
	ID3D11Buffer* verBuf;
	ID3D11Buffer* iBuf;
};