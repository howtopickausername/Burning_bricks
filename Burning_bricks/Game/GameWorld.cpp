#include "stdafx.h"
#include "GameWorld.h"

cGameWorld::cGameWorld()
{
}

void cGameWorld::WorldBeginning()
{
	KbCmds = std::make_shared<cKbCmdProductor>();
	cLocator::InputLayer().GetKeyboard()->SetReceiver(KbCmds);
	m_CurScene = std::make_shared<cGameScene>();
	m_CurScene->CreateScene();
}

void cGameWorld::HandleInput()
{
	cLocator::InputLayer().UpdateDevice();
	auto& cmdQ = KbCmds->CmdQ;
	while (!cmdQ.empty()) {
		m_CurScene->HandleInput(cmdQ.front());
		cmdQ.pop();
	}
}

void cGameWorld::Update()
{
	m_CurScene->Update();
}

void cGameWorld::Render()
{
}
