#include "stdafx.h"
#include "GameWorld.h"

cGameWorld::cGameWorld()
{
}

void cGameWorld::WorldBeginning()
{
	KbCmds = std::make_shared<cKbCmdProductor>();
	KbCmds->CmdDownMaps[DIK_W] = cCommand(eGoForward);
	KbCmds->CmdDownMaps[DIK_S] = cCommand(eRetreat);
	KbCmds->CmdDownMaps[DIK_A] = cCommand(eTurnLeft);
	KbCmds->CmdDownMaps[DIK_D] = cCommand(eTurnRight);
	KbCmds->CmdDownMaps[DIK_J] = cCommand(eJump);
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
	m_CurScene->ProcessCmd();
}

void cGameWorld::Update()
{
	m_CurScene->Update();
}

void cGameWorld::Render()
{
}
