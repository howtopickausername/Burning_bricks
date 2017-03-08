#include "stdafx.h"
#include "GameObject.h"


cGameObject::cGameObject(cState const& basic_state) {
	State.push_back(basic_state.Copy());
	for (auto const &s : State) {
		s->Init(*this);
	}
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
	//使用ModelInfo绘制
	canvas->begin();
	std::map<ePropTypes, uPropValue>::iterator prop = Props.find(ePropTypes::eHp);
 	if (prop != Props.end()) {
 		float left = 100;
 		float top = 100;
 		float width = 100;
 		float height = 20;
 		canvas->DrawRect(left, top, left + width, top + height, 1);
 		canvas->DrawRect(left, top, left + width * prop->second.fv, top + height, 2);
 	}
	std::wstring txt;
	if (Models[0].model.resId == 1) {
		txt = L"站立";
	}
	else if(Models[0].model.resId == 2) {
		txt = L"行走";
	}
	canvas->DrawTextW(0, 0, 100, 50, txt);
	canvas->end();
}

cGameObject::pProp const& cGameObject::GetProps() const {
	return Props;
}

cGameObject::pProp & cGameObject::GetBackProps() {
	return BackProps;
}

std::array<cModelInfo, 3>& cGameObject::GetModels()
{
	return Models;
}

std::shared_ptr<cGameObject> cGameObject::Copy() {
	return std::make_shared<cGameObject>(*this);
}
