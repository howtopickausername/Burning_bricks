#pragma once
#include <vector>
#include <d3d11.h>
#include "DirectXTK/Inc/SimpleMath.h"
#include "GraphicsDefine.h"

_GP_BEGIN;
class cCamera
{
public:
	cCamera();
	~cCamera();

	DirectX::XMMATRIX ViewMatrix();
	DirectX::XMMATRIX ProjMatrix(int width, int height);
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 dir;
	DirectX::XMFLOAT3 up;
	float fov;
	float nearPlane;
	float farPlane;
};

typedef std::vector<cCamera> cCameras;
_GP_END;