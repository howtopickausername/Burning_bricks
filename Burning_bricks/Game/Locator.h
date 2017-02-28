#pragma once
#include "Audio/Audio.h"
#include "Physics/Physics.h"
#include "Graphics/Graphics.h"
#include "ToolKits/Log.h"

class cLocator
{
public:
	static cLog& Log();
	static cAudio& Audio();
	static cPhysics& Physics();
	static cGraphics& Graphics();

private:
	static cLog* m_Log;
	static cAudio* m_audio;
	static cPhysics* m_physics;
	static cGraphics* m_Graphics;
};