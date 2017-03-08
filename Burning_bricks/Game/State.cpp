#include "stdafx.h"
#include "State.h"


cState::cState(pStateMap map)
	:StateMap(map)
{
	StateBreed = std::make_shared<cStateBreed>(StateMap->At(0));
}

cState::~cState()
{

}

void cState::Init(cGameObject & obj) {
	StateBreed->fPre(obj);
}

void cState::Update(cGameObject& obj)
{
	StateBreed->fUpdate(obj);
}

void cState::HandleInput(cGameObject& obj, pCommand cmd)
{
	auto rt = StateBreed->fHandleInput(obj, cmd);
	if (std::get<0>(rt) == cStateBreed::OP::eRoleBack){
		auto oldId = StateStack.top();
		StateStack.pop();
		StateBreed->fPost(obj);
		StateBreed = std::make_shared<cStateBreed>(StateMap->At(oldId));
		StateBreed->fPre(obj);
	}
	else if (std::get<0>(rt) == cStateBreed::OP::eNew) {
		auto newBreed = StateMap->At(std::get<1>(rt));
		StateBreed->fPost(obj);
		StateStack.push(StateBreed->Id);
		StateBreed = std::make_shared<cStateBreed>(newBreed);
		StateBreed->fPre(obj);
	}
	else if (std::get<0>(rt) == cStateBreed::OP::eConstant) {
		//do nothing
	}
}

const pStateBreed cState::GetBreed() const {
	return StateBreed;
}

std::shared_ptr<cState> cState::Copy() const
{
	return std::make_shared<cState>(*this);
}