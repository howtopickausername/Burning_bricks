#pragma once
#include "Locator.h"
#include "GameScene.h"
#include "KetboardCmdProductor.h"

class cGameWorld
{
public:
	cGameWorld();
	void WorldBeginning();
	void HandleInput();
	void Update();
	void Render();

protected:
	pKbCmdproductor KbCmds;
	pGameScene m_CurScene;
};
typedef std::shared_ptr<cGameWorld> pGameWorld;