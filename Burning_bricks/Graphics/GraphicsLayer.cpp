#include "stdafx.h"
#include "GraphicsLayer.h"
#include "GraphicsException.h"
#include "Locator.h"

const UINT cGraphicsLayer::m_uiMAX_CHARS_PER_FRAME = 512;
cGraphicsLayer::cGraphicsLayer()
	:m_StencilRefStored11(0)
{
}

cGraphicsLayer::~cGraphicsLayer()
{
	RELEASE(m_BlendState);
	RELEASE(m_pDepthStencilState);
	RELEASE(m_pDepthStencilView);
	RELEASE(m_pDepthStencilBuffer);
	RELEASE(m_pRenderTargetView);
	RELEASE(m_pBackBuffer);
	RELEASE(m_pSwapChain);
	RELEASE(m_pMessageQueue);
	RELEASE(m_Context);

	//Release D2d////////////////////////////////////////////////////////////////////////
	RELEASE(m_pDirect2DFactory);
	RELEASE(m_pD2dRenderTarget);
	RELEASE(m_pLightSlateGrayBrush);
	RELEASE(m_pCornflowerBlueBrush);
	ReportLiveDeviceObjects();
	RELEASE(m_Device);
}

void cGraphicsLayer::Init(HWND hWnd, int width, int height)
{
	cGraphics::Init(hWnd, width, height);
	CreateDeviceAndSwapChain();
	CreateDepthStencilBuffer();
	CreateStates();
	//D2d
	Create2DRsource();
}

void cGraphicsLayer::DumpMessages()
{
	assert(m_pMessageQueue);

	HRESULT r = 0;
	int iCount = 0;
	while (1)
	{
		iCount++;
		if (iCount > 10)
			break;
		SIZE_T messageLength = 0;
		r = m_pMessageQueue->GetMessageW(0, nullptr, &messageLength);
		if (messageLength == 0)
		{
			break;
		}
		D3D11_MESSAGE * pMessage = (D3D11_MESSAGE*)malloc(messageLength);
		r = m_pMessageQueue->GetMessageW(0, pMessage, &messageLength);
		if (FAILED(r))
		{
			OutputDebugString(L"Failed to get Direct3D Message!\n");
			break;
		}

		std::wstringstream ss;
		ss << "D3DMSG [Cat[" << pMessage->Category << "]] ";
		ss << "Sev[" << pMessage->Severity << "] ";
		ss << "ID[" << pMessage->ID << "]:";
		ss << pMessage->pDescription << "\n";
		OutputDebugString(ss.str().c_str());
	}
}

void cGraphicsLayer::Present()
{
	HRESULT r = S_OK;
	assert(m_Device);
	r = m_pSwapChain->Present(0, 0);
	if (FAILED(r))
	{
		OutputDebugString(L"Present Failed!\n");
	}
	DumpMessages();
}

void cGraphicsLayer::Clear(const float(&colClear)[4])
{
	m_Context->ClearRenderTargetView(m_pRenderTargetView, colClear);
}

void cGraphicsLayer::ClearDepthStencil(float fDepth, int uiStencil)
{
	m_Context->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, fDepth, uiStencil);
}

void cGraphicsLayer::CreateDeviceAndSwapChain()
{
	HRESULT r = 0;
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(swapDesc));

	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = m_width;
	swapDesc.BufferDesc.Height = m_height;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_Wnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Windowed = TRUE;

	r = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapDesc,
		&m_pSwapChain,
		&m_Device,
		nullptr,
		&m_Context);

	if (FAILED(r))
	{
		throw cGraphicsExcp("Could not create IDirect3DDevice11\n");
	}

	r = m_Device->QueryInterface(__uuidof(ID3D11InfoQueue), (LPVOID*)&m_pMessageQueue);
	if (FAILED(r))
	{
		throw cGraphicsExcp("Could not create IDirect3DDevice11 message queue!\n");
	}
	m_pMessageQueue->SetMuteDebugOutput(false);
	m_pMessageQueue->SetMessageCountLimit(-1);

	r = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&m_pBackBuffer));
	if (FAILED(r))
	{
		throw cGraphicsExcp("Could not get back buffer!\n");
	}

	r = m_Device->CreateRenderTargetView(m_pBackBuffer, nullptr, &m_pRenderTargetView);
	if (FAILED(r))
	{
		throw cGraphicsExcp("Could not create render target view\n");
	}
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(m_width);
	vp.Height = static_cast<FLOAT>(m_height);
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_Context->RSSetViewports(1, &vp);
}

void cGraphicsLayer::CreateDepthStencilBuffer()
{
	HRESULT hr = 0;
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D10_TEXTURE2D_DESC));
	descDepth.Width = m_width;
	descDepth.Height = m_height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_Device->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencilBuffer);
	if (FAILED(hr))
	{
		throw cGraphicsExcp("Unable to create depth buffer!\n");
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
	ZeroMemory(&descDSView, sizeof(D3D10_DEPTH_STENCIL_VIEW_DESC));
	descDSView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSView.Texture2D.MipSlice = 0;

	hr = m_Device->CreateDepthStencilView(m_pDepthStencilBuffer, &descDSView, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		throw cGraphicsExcp("Could not create depth / stencil view!\n");
	}
	m_Context->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}

void cGraphicsLayer::ReportLiveDeviceObjects()
{
#if	defined(DEBUG) || defined(_DEBUG)
	ID3D11Debug *d3dDebug;
	HRESULT hr = m_Device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hr))
	{
		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	if (d3dDebug != nullptr)			d3dDebug->Release();
#endif
}

void cGraphicsLayer::CreateStates()
{
	D3D11_DEPTH_STENCIL_DESC descDS;
	ZeroMemory(&descDS, sizeof(D3D10_DEPTH_STENCIL_DESC));
	descDS.DepthEnable = TRUE;
	descDS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDS.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	descDS.StencilEnable = TRUE;
	descDS.StencilReadMask = (UINT8)0xFFFFFFFF;
	descDS.StencilWriteMask = (UINT8)0xFFFFFFFF;

	descDS.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
	descDS.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	descDS.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	descDS.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	descDS.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	descDS.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	descDS.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	descDS.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT hr = m_Device->CreateDepthStencilState(&descDS, &m_pDepthStencilState);
	if (FAILED(hr))
	{
		throw cGraphicsExcp("Could not create depth/stencil!\n");
	}
	D3D11_BLEND_DESC blendState;
	ZeroMemory(&blendState, sizeof(D3D10_BLEND_DESC1));
	blendState.RenderTarget[0].BlendEnable = true;
	blendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_Device->CreateBlendState(&blendState, &m_BlendState);
	if (FAILED(hr))
	{
		throw cGraphicsExcp("CreateAndSetBlendState failed");
	}
}

void cGraphicsLayer::SetStates(ID3D11DeviceContext* context)
{
	context->OMSetDepthStencilState(m_pDepthStencilState, m_StencilRefStored11);
	context->OMSetBlendState(m_BlendState, nullptr, 0xffffffff);
}

void cGraphicsLayer::Create2DRsource()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2DFactory);
	if (FAILED(hr))
	{
		throw cGraphicsExcp("Create2DRsource - D2d1CreateFactory failed.");
	}

	RECT rc;
	GetClientRect(m_Wnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	hr = m_pDirect2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_Wnd, size),
		&m_pD2dRenderTarget);
	if (FAILED(hr))
	{
		throw cGraphicsExcp("Create2DRsource - CreateHwndRenderTarget failed.");
	}
	hr = m_pD2dRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::LightSlateGray),
		&m_pLightSlateGrayBrush);
	hr = m_pD2dRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
		&m_pCornflowerBlueBrush);
}

void cGraphicsLayer::DiscardDeviceResource()
{
	RELEASE(m_pD2dRenderTarget);
	RELEASE(m_pLightSlateGrayBrush);
	RELEASE(m_pCornflowerBlueBrush);
}

ID2D1HwndRenderTarget & cGraphicsLayer::Get2dRt() {
	return *m_pD2dRenderTarget;
}

void cGraphicsLayer::D2DRender()
{
	m_pD2dRenderTarget->BeginDraw();
	m_pD2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pD2dRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	D2D1_SIZE_F rtSize = m_pD2dRenderTarget->GetSize();
	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);
	for (int x = 0; x < width ; x+=10)
	{
		m_pD2dRenderTarget->DrawLine(
			D2D1::Point2F(static_cast<float>(x), 0.0f),
			D2D1::Point2F(static_cast<float>(x), rtSize.height),
			m_pLightSlateGrayBrush,
			0.5f);
	}
	for (int y = 0; y < width; y += 10)
	{
		m_pD2dRenderTarget->DrawLine(
			D2D1::Point2F(0.0f, static_cast<float>(y)),
			D2D1::Point2F( rtSize.width, static_cast<float>(y)),
			m_pLightSlateGrayBrush,
			0.5f);
	}
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		rtSize.width / 2 - 50.f,
		rtSize.height / 2 - 50.0f,
		rtSize.width / 2 + 50.0f,
		rtSize.height / 2 + 50.0f
		);

	D2D1_RECT_F rectangle2 = D2D1::RectF(
		rtSize.width / 2 - 100.0f,
		rtSize.height / 2 - 100.0f,
		rtSize.width / 2 + 100.0f,
		rtSize.height / 2 + 100.0f
		);
	m_pD2dRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);
	m_pD2dRenderTarget->FillRectangle(&rectangle2, m_pCornflowerBlueBrush);

	HRESULT hr = m_pD2dRenderTarget->EndDraw();
	if (FAILED(hr))
	{
		cLocator::Log() << "D2DRender failed" << hr;
	}
}

void cGraphicsLayer::Release()
{
	RELEASE(m_BlendState);
	RELEASE(m_pDepthStencilState);
	RELEASE(m_pDepthStencilView);
	RELEASE(m_pDepthStencilBuffer);
	RELEASE(m_pRenderTargetView);
	RELEASE(m_pBackBuffer);
	RELEASE(m_pSwapChain);
	RELEASE(m_pMessageQueue);
	RELEASE(m_Context);

	//Release D2d////////////////////////////////////////////////////////////////////////
	RELEASE(m_pDirect2DFactory);
	RELEASE(m_pD2dRenderTarget);
	RELEASE(m_pLightSlateGrayBrush);
	RELEASE(m_pCornflowerBlueBrush);
	ReportLiveDeviceObjects();
	RELEASE(m_Device);
}

pCanvas cGraphicsLayer::NewCanvas(int width, int height)
{
	return std::make_shared<c2dCanvas>();
}

void cGraphicsLayer::Draw()
{
	D2DRender();
}

void c2dCanvas::begin() {
	cGraphicsLayer& lay = dynamic_cast<cGraphicsLayer&>(cLocator::Graphics());
	lay.Get2dRt().BeginDraw();
	lay.Get2dRt().SetTransform(D2D1::Matrix3x2F::Identity());
	lay.Get2dRt().Clear(D2D1::ColorF(D2D1::ColorF::White));
}

void c2dCanvas::DrawRect(float left, float top, float right, float bottom, int color) {
	cGraphicsLayer& lay = dynamic_cast<cGraphicsLayer&>(cLocator::Graphics());
	D2D1_RECT_F rectangle1 = D2D1::RectF(left, top, right, bottom);
	ID2D1SolidColorBrush * brush = nullptr;
	switch (color) {
	case 1:
		brush = lay.m_pLightSlateGrayBrush;
		break;
	case 2:
		brush = lay.m_pCornflowerBlueBrush;
		break;
	default:
		brush = lay.m_pLightSlateGrayBrush;
		break;
	}
	lay.Get2dRt().FillRectangle(&rectangle1, brush);
}

void c2dCanvas::end() {
	cGraphicsLayer& lay = dynamic_cast<cGraphicsLayer&>(cLocator::Graphics());
	HRESULT hr = lay.Get2dRt().EndDraw();
}
