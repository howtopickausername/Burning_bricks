#include "stdafx.h"
#include "Camera.h"


Gp::cCamera::~cCamera()
{

}

Gp::cCamera::cCamera()
	:pos(DirectX::SimpleMath::Vector3::UnitZ * 5),
	dir(DirectX::SimpleMath::Vector3::UnitZ * -1),
	up(DirectX::SimpleMath::Vector3::UnitY),
	fov(DirectX::XM_PIDIV4),
	nearPlane(0.01),
	farPlane(1000)
{

}

DirectX::XMMATRIX Gp::cCamera::ViewMatrix()
{
	using namespace DirectX::SimpleMath;
	return Matrix::CreateLookAt(pos, pos + dir, Vector3::Up);
// 	auto view = Matrix::CreateWorld(pos, dir, Vector3::Up);
// 	return view.Invert();
}

DirectX::XMMATRIX Gp::cCamera::ProjMatrix(int width, int height)
{
	using namespace DirectX::SimpleMath;
	return Matrix::CreatePerspectiveFieldOfView(
		DirectX::XM_PIDIV4,
		1.0f * width / height,
		nearPlane, farPlane);
}
