#include "stdafx.h"
#include "GameWorld.h"

cGameWorld::cGameWorld()
{
}

void cGameWorld::WorldBeginning()
{
	cCommand<int, float, std::string> ccc;
	ccc.Get<1>() = 10;
	KbCmds = std::make_shared<cKbCmdProductor>();
	KbCmds->CmdDownMaps[DIK_W] = cCmdBase(eGoForward);
	KbCmds->CmdDownMaps[DIK_S] = cCmdBase(eRetreat);
	KbCmds->CmdDownMaps[DIK_A] = cCmdBase(eTurnLeft);
	KbCmds->CmdDownMaps[DIK_D] = cCmdBase(eTurnRight);
	KbCmds->CmdDownMaps[DIK_J] = cCmdBase(eJump);
	KbCmds->CmdDownMaps[DIK_ESCAPE] = cCmdBase(eTurnBack);
	KbCmds->CmdDownMaps[DIK_Q] = cCmdBase(eSkill);
	KbCmds->CmdUpMaps = KbCmds->CmdDownMaps;
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
}
