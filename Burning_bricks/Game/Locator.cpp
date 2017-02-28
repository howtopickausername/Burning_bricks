#include "stdafx.h"
#include "Locator.h"

cLog* cLocator::m_Log = nullptr;
cAudio* cLocator::m_audio = nullptr;
cPhysics* cLocator::m_physics = nullptr;
cGraphics* cLocator::m_Graphics = nullptr;

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
