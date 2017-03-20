#pragma once
#include "Audio/Audio.h"
#include "Physics/Physics.h"
#include "Graphics/Graphics.h"
#include "ToolKits/Log.h"
#include "Controler/InputLayer.h"
#include "Graphics/ModelRes.h"

class cLocator
{
public:
	static void Init(cLog* log, cAudio* audio, cPhysics* phys, cGraphics* grap, cInputLayer* inputlayer, Gp::cModelRes* modelRes);
	static cLog& Log();
	static cAudio& Audio();
	static cPhysics& Physics();
	static cGraphics& Graphics();
	static cInputLayer& InputLayer();
	static Gp::cModelRes& ModelRes();

private:
	static cLog* m_Log;
	static cAudio* m_audio;
	static cPhysics* m_physics;
	static cGraphics* m_Graphics;
	static cInputLayer* m_InputLayer;
	static Gp::cModelRes* m_ModelRes;
	static bool m_Init;
};