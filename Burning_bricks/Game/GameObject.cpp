#include "stdafx.h"
#include "GameObject.h"
#include "Locator.h"

cModelInfo::cModelInfo()
	:model(nullptr),
	transform(DirectX::SimpleMath::Matrix::Identity)
{

}

cGameObject::cGameObject(IdType id, cState const& basic_state) 
	:Id(id)
{
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
	/*canvas->begin();
	std::map<ePropTypes, cPropValue>::iterator prop = Props.find(ePropTypes::eHp);
 	if (prop != Props.end()) {
 		float left = 100;
 		float top = 100;
 		float width = 100;
 		float height = 20;
 		canvas->DrawRect(left, top, left + width, top + height, 1);
 		canvas->DrawRect(left, top, left + width * prop->second.fv, top + height, 2);
 	}
	prop = Props.find(ePropTypes::eCustom);
	if (prop != Props.end()){
		float left = 100;
		float top = 150;
		float width = 100;
		float height = 20;
		canvas->DrawRect(left, top, left + width, top + height, 1);
		canvas->DrawRect(left, top, left + width * prop->second.fv, top + height, 2);
	}
	std::wstring txt;
	if (Models[0].model.resName == L"1") {
		txt = L"站立";
	}
	else if(Models[0].model.resName == L"2") {
		txt = L"行走";
	}
	else if (Models[0].model.resName == L"3") {
		txt = L"技能";
	}
	canvas->DrawTextW(0, 0, 100, 50, txt);
	canvas->end();*/

	cGraphics& gp = cLocator::Graphics();
	DirectX::XMMATRIX viewM = gp.GetCameras()[0].ViewMatrix();
	DirectX::XMMATRIX projM = gp.GetCameras()[0].ProjMatrix(gp.Width(), gp.Height());
	for (auto mInfo : Models)
	{
		if (mInfo.model == nullptr){
			continue;
		}
		using namespace DirectX::SimpleMath;
		DirectX::XMMATRIX world = Matrix::Identity;
		DirectX::Model* model = mInfo.model;
		DirectX::XMMATRIX worldM = mInfo.transform;
		model->Draw(gp.Contex(), gp.GetCommonStates(), worldM, viewM, projM);
	}
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
