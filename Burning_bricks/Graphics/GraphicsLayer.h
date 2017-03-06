#pragma once
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include "Graphics.h"

class cGraphicsLayer : public cGraphics
{
public:
	cGraphicsLayer();
	virtual ~cGraphicsLayer();

	void Init(HWND hWnd, int width, int height);
	virtual void Release() override;
	virtual pCanvas NewCanvas(int width, int height) override;
	virtual void Draw() override;
	void Present() override;
	void Clear(const float(&colClear)[4]) override;
	virtual void ClearDepthStencil(float fDepth, int uiStencil) override;

protected:
	void CreateDeviceAndSwapChain();
	void CreateDepthStencilBuffer();
	void ReportLiveDeviceObjects();
	void CreateStates();
	void SetStates(ID3D11DeviceContext* context);

	//D2d////////////////////////////////////////////////////////////////////////
	void Create2DRsource();
	void DiscardDeviceResource();
	void D2DRender();

	void DumpMessages();

protected:
	ID3D11Device *m_Device;
	ID3D11DeviceContext* m_Context;
	ID3D11Texture2D* m_pBackBuffer;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	UINT m_StencilRefStored11;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RenderTargetView* m_pRenderTargetView;
	IDXGISwapChain* m_pSwapChain;
	ID3D11BlendState* m_BlendState;
	
	ID3D11InfoQueue* m_pMessageQueue;
	static const UINT m_uiMAX_CHARS_PER_FRAME;

	//D2D////////////////////////////////////////////////////////////////////////
	ID2D1Factory *m_pDirect2DFactory;
	ID2D1HwndRenderTarget *m_pD2dRenderTarget;
	ID2D1SolidColorBrush *m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush *m_pCornflowerBlueBrush;
};