#include "stdafx.h"
#include "GameObject.h"


cGameObject::cGameObject(cState const& basic_state) {
	State.push_back(basic_state.Copy());
	uPropValue v;
	v.fv = 1;
	Props[ePropTypes::eHp] = v;
	BackProps = Props;
}

cGameObject::~cGameObject() {

}

void cGameObject::SendCmd(pCommand cmd) {
	CmdQ.push(cmd);
}

void cGameObject::Update() {
	for (auto const &s : State) {
		s->Update(*this);
	}
}

void cGameObject::ProcessCmd() {
	while (!CmdQ.empty()) {
		auto cmd = CmdQ.front();
		CmdQ.pop();
		for (auto const& s : State) {
			s->HandleInput(*this, cmd);
		}
	}
}

void cGameObject::Swap() {
	Props = BackProps;
}

void cGameObject::Render(pCanvas & canvas) {
	// π”√ModelInfoªÊ÷∆
	std::map<ePropTypes, uPropValue>::iterator prop = Props.find(ePropTypes::eHp);
	if (prop != Props.end()) {
		canvas->begin();
		float left = 100;
		float top = 100;
		float width = 100;
		float height = 20;
		canvas->DrawRect(left, top, left + width, top + height, 1);
		canvas->DrawRect(left, top, left + width * prop->second.fv, top + height, 2);
		canvas->DrawTextW(left, top, left + width * prop->second.fv, top + height, L"hello world");
		canvas->end();
	}
}

cGameObject::pProp const& cGameObject::GetProps() const{
	return Props;
}

cGameObject::pProp & cGameObject::GetBackProps() {
	return BackProps;
}

std::shared_ptr<cGameObject> cGameObject::Copy() {
	return std::make_shared<cGameObject>(*this);
}
