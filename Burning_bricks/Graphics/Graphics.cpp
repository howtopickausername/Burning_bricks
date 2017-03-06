#include "stdafx.h"
#include "Graphics.h"

cGraphics::cGraphics()
	:m_Wnd(nullptr), m_width(0), m_height(0)
{

}

cGraphics::~cGraphics()
{

}

void cGraphics::Init(HWND hWnd, int width, int height)
{
	m_Wnd = hWnd;
	m_width = width;
	m_height = height;
}

void cGraphics::Release()
{

}

int cGraphics::Width() const
{
	return m_width;
}

int cGraphics::Height() const
{
	return m_height;
}

pCanvas cGraphics::NewCanvas(int width, int height)
{
	return std::make_shared<cCanvas>();
}

void cGraphics::Draw()
{
}

void cGraphics::Present()
{

}

void cGraphics::Clear(const float(&colClear)[4])
{

}

void cGraphics::ClearDepthStencil(float fDepth, int uiStencil)
{

}
