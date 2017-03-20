#pragma once
#include <memory>
#include <d3d11.h>
#include "Canvas.h"
#include "Effect.h"
#include "Model.h"
#include "DirectXTK/Inc/Effects.h"
#include "DirectXTK/Inc/CommonStates.h"
#include "Camera.h"

class cGraphics
{
public:
	cGraphics();
	virtual ~cGraphics();
	virtual void Init(HWND hWnd, int width, int height);
	virtual void Release();

	virtual ID3D11Device* Device() = 0;
	virtual ID3D11DeviceContext* Contex() = 0;
	virtual int Width() const;
	virtual int Height() const;

	virtual pCanvas NewCanvas(int width, int height);
	virtual void Draw();
	virtual void Draw(Gp::cModel& model, Gp::cEffect& effect);
	virtual void Present();
	virtual void Clear(const float(&colClear)[4]);
	virtual void ClearDepthStencil(float fDepth, int uiStencil);
	virtual DirectX::EffectFactory& GetEffFactory() = 0;
	virtual DirectX::CommonStates& GetCommonStates() = 0;
	virtual Gp::cCameras& GetCameras();

protected:
	HWND m_Wnd;
	int m_width;
	int m_height;
	Gp::cCameras m_Cameras;
};
typedef std::shared_ptr<cGraphics> pGraphics;