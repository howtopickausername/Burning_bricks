#include "stdafx.h"
#include "Graphics.h"



cGraphics::~cGraphics()
{

}

void cGraphics::Init(HWND hWnd, int width, int height)
{
	m_hWnd = hWnd;
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

void cGraphics::Draw(cDdInfo const& info)
{
	info;
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
