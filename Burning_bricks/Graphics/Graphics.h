#pragma once
#include <memory>
#include "Canvas.h"

class cGraphics
{
public:
	cGraphics();
	virtual ~cGraphics();
	virtual void Init(HWND hWnd, int width, int height);
	virtual void Release();

	virtual int Width() const;
	virtual int Height() const;

	virtual pCanvas NewCanvas(int width, int height);
	virtual void Draw();
	virtual void Present();
	virtual void Clear(const float(&colClear)[4]);
	virtual void ClearDepthStencil(float fDepth, int uiStencil);

protected:
	HWND m_Wnd;
	int m_width;
	int m_height;
};
typedef std::shared_ptr<cGraphics> pGraphics;