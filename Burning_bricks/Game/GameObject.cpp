#include "stdafx.h"
#include "GameObject.h"


cGameObject::cGameObject(cState const& basic_state)
{
	State.push_back(basic_state.Copy());
}

cGameObject::~cGameObject()
{

}

void cGameObject::SendCmd(pCommand cmd)
{
	CmdQ.push(cmd);
}

void cGameObject::Update()
{
	for (auto const &s : State) {
		s->Update();
	}
}

void cGameObject::ProcessCmd()
{
	while (!CmdQ.empty()) {
		auto cmd = CmdQ.front();
		CmdQ.pop();
		for (auto const& s : State) {
			s->HandleInput(cmd);
		}
	}
}

void cGameObject::Swap()
{
	Props.swap(BackProps);
}

void cGameObject::Render(pCanvas & canvas)
{
	// π”√ModelInfoªÊ÷∆
}

cGameObject::pProp const& cGameObject::GetProps() const
{
	return Props;
}

std::shared_ptr<cGameObject> cGameObject::Copy()
{
	return std::make_shared<cGameObject>(*this);
}