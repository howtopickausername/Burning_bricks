#pragma once
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

class cGraphicsLayer
{
public:
	~cGraphicsLayer();

	void InitD3D(int width, int height, int bpp);
	static void Create(HWND hWnd, short width, short height);

	static cGraphicsLayer* GetGraphics() { return m_GraphicsLayer; }
	inline ID3D11Device* GetDevice() { return m_Device; }
	inline ID3D11DeviceContext* GetContext() { return m_Context; }

	ID3D11Texture2D* GetBackBuffer() { return m_pBackBuffer; }

	inline int Width() { return m_rcScreenRect.right; }
	inline int Height() { return m_rcScreenRect.bottom; }

	void Present();
	void Clear(const float(&colClear)[4]);
	void ClearDepthStencil(const float fDepth, const UINT8 uiStencil);
	void DestroyAll();
	
	//D2d////////////////////////////////////////////////////////////////////////
	void D2DRender();

protected:
	void CreateDeviceAndSwapChain(int width, int height, int bpp);
	void CreateDepthStencilBuffer();
	void ReportLiveDeviceObjects();
	void CreateStates();
	void SetStates(ID3D11DeviceContext* context);

	//D2d////////////////////////////////////////////////////////////////////////
	void Create2DRsource();
	void DiscardDeviceResource();

	void DumpMessages();

protected:
	cGraphicsLayer(HWND hWnd);
	static cGraphicsLayer* m_GraphicsLayer;
	HWND m_Hwnd;
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
	RECT m_rcScreenRect;
	
	ID3D11InfoQueue* m_pMessageQueue;
	static const UINT m_uiMAX_CHARS_PER_FRAME;

	//D2D////////////////////////////////////////////////////////////////////////
	ID2D1Factory *m_pDirect2DFactory;
	ID2D1HwndRenderTarget *m_pD2dRenderTarget;
	ID2D1SolidColorBrush *m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush *m_pCornflowerBlueBrush;
};

cGraphicsLayer* Graphics();