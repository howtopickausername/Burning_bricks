#pragma once
#include "Graphics/Canvas.h"
#include "Locator.h"
#include "GameScene.h"
#include "KetboardCmdProductor.h"
#include "MouseCmdProductor.h"
#include "Resource/Model.h"
#include "Graphics/Effect.h"

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
	pMsCmdProductor MsCmds;
	pCanvas m_SceneCanvas;
	//pModel m_Model;
	Gp::pEffect m_Effect;
	pGameScene m_CurScene;
	
	std::unique_ptr<DirectX::Model> m_uModel;
};
typedef std::shared_ptr<cGameWorld> pGameWorld;