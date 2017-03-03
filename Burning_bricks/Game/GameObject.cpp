#include "stdafx.h"
#include "GameObject.h"


cGameObject::cGameObject(cState const& basic_state)
{
	BackState.push_back(basic_state.Copy());
	CurState.push_back(basic_state.Copy());
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
	for (auto const &s : BackState) {
		s->Update();
	}
}

void cGameObject::ProcessCmd()
{
	while (!BackCmdQ.empty()) {
		auto cmd = BackCmdQ.front();
		BackCmdQ.pop();
		for (auto const& s : BackState) {
			s->HandleInput(cmd);
		}
	}
}

void cGameObject::Swap()
{
	CurCmdQ.swap(BackCmdQ);
	CurState.swap(BackState);
}

const pState cGameObject::GetCommonState() const {
	assert(CurState.size() > 0);
	return CurState[0];
}

std::shared_ptr<cGameObject> cGameObject::Copy()
{
	return std::make_shared<cGameObject>(*this);
}
