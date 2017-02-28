#include "stdafx.h"
#include "GameObject.h"


cGameObject::cGameObject(cState const& state)
{
	BackState = state.Copy();
	CurState = state.Copy();
}

cGameObject::~cGameObject()
{

}

void cGameObject::SendCmd(cCommand const& cmd)
{
	CurCmdQ.push(cmd);
}

void cGameObject::Update()
{
	BackState->Update();
}

void cGameObject::HandleInput()
{
	while (!BackCmdQ.empty()) {
		auto cmd = BackCmdQ.front();
		BackCmdQ.pop();
		BackState->HandleInput(cmd);
	}
}

void cGameObject::Swap()
{
	CurCmdQ.swap(BackCmdQ);
	CurState.swap(BackState);
}

std::shared_ptr<cGameObject> cGameObject::Copy()
{
	return std::make_shared<cGameObject>(*this);
}
