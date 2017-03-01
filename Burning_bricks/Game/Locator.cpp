#include "stdafx.h"
#include "Locator.h"

cLog* cLocator::m_Log = nullptr;
cAudio* cLocator::m_audio = nullptr;
cPhysics* cLocator::m_physics = nullptr;
cGraphics* cLocator::m_Graphics = nullptr;
cInputLayer* cLocator::m_InputLayer = nullptr;
bool cLocator::m_Init = false;

void cLocator::Init(cLog * log, cAudio * audio, cPhysics * phys, cGraphics * grap, cInputLayer* inputlayer)
{
	assert(!m_Init && "cLocator already initialized");
	m_Log = log;
	m_audio = audio;
	m_physics = phys;
	m_Graphics = grap;
	m_InputLayer = inputlayer;
	m_Init = true;
}

cLog& cLocator::Log()
{
	return *m_Log;
}

cAudio& cLocator::Audio()
{
	return *m_audio;
}

cPhysics& cLocator::Physics()
{
	return *m_physics;
}

cGraphics& cLocator::Graphics()
{
	return *m_Graphics;
}

cInputLayer & cLocator::InputLayer()
{
	return *m_InputLayer;
}
