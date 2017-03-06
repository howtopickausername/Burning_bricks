#include "stdafx.h"
#include "GameWorld.h"

cGameWorld::cGameWorld()
{
}

void cGameWorld::WorldBeginning()
{
	KbCmds = std::make_shared<cKbCmdProductor>();
	KbCmds->CmdDownMaps[DIK_W] = std::make_shared<cCommand>(eGoForward);
	KbCmds->CmdDownMaps[DIK_S] = std::make_shared<cCommand>(eRetreat);
	KbCmds->CmdDownMaps[DIK_A] = std::make_shared<cCommand>(eTurnLeft);
	KbCmds->CmdDownMaps[DIK_D] = std::make_shared<cCommand>(eTurnRight);
	KbCmds->CmdDownMaps[DIK_J] = std::make_shared<cCommand>(eJump);
	cLocator::InputLayer().GetKeyboard()->SetReceiver(KbCmds);
	m_SceneCanvas = cLocator::Graphics().NewCanvas(cLocator::Graphics().Width(), cLocator::Graphics().Height());
	m_CurScene = std::make_shared<cGameScene>();
	m_CurScene->CreateScene();
}

void cGameWorld::HandleInput()
{
	cLocator::InputLayer().UpdateDevice();
	auto& cmdQ = KbCmds->CmdQ;
	while (!cmdQ.empty()) {
		m_CurScene->DispatchCmd(cmdQ.front());
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
	m_CurScene->Render(m_SceneCanvas);
	cLocator::Graphics().Draw();
}
