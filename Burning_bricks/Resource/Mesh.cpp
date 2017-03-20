#include "stdafx.h"
#include <string>
#include <DirectXMath.h>
#include "Mesh.h"
#include "ResourceExcepthion.h"


cMesh::cMesh(ID3D11Device& device, std::vector<cVertex> const& v, std::vector<short> const& i)
	:vertices(v), indices(i)
{
	this->InitBuf(device);
}

cMesh::~cMesh()
{

}

void cMesh::Draw(ID3D11DeviceContext& context, Gp::cEffect& effect)
{
	context.IASetInputLayout(effect.GetInputLayout(0, 0));
	DirectX::XMVECTOR eyepos = DirectX::XMVectorSet(0, 0, -5, 0);
	DirectX::XMVECTOR focuspos = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMVECTOR updir = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMMATRIX mtworld = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX mtview = DirectX::XMMatrixLookAtLH(eyepos, focuspos, updir);
	mtview = XMMatrixTranspose(mtview);
	DirectX::XMMATRIX mtproj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 0.6, 0.01, 10000);
	mtproj = XMMatrixTranspose(mtproj);
	effect.m_mtxWorldVar->SetMatrix((float*)&mtworld);
	effect.m_mtxViewVar->SetMatrix((float*)&mtview);
	effect.m_mtxProjVar->SetMatrix((float*)&mtproj);

	UINT uiStride = sizeof(cVertex);
	UINT uiOffset = 0;
	context.IASetVertexBuffers(0, 1, &verBuf, &uiStride, &uiOffset);
	context.IASetIndexBuffer(iBuf, DXGI_FORMAT_R16_UINT, 0);
	context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	effect.GetEffect().GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, &context);
	context.DrawIndexed(indices.size(), 0, 0);
}

void cMesh::InitBuf(ID3D11Device& device)
{
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
	HRESULT hr = device.CreateBuffer(&descBuf, &resData, &verBuf);
	if (FAILED(hr)){
		throw cResourceExcep("Mesh CreateVertexBuffer failed." + std::to_string(hr));
	}

	descBuf.ByteWidth = sizeof(short) * indices.size();
	descBuf.BindFlags = D3D11_BIND_INDEX_BUFFER;
	resData.pSysMem = &indices[0];
	hr = device.CreateBuffer(&descBuf, &resData, &iBuf);
	if (FAILED(hr)) {
		throw cResourceExcep("Mesh CreateIndexBuffer failed." + std::to_string(hr));
	}
}