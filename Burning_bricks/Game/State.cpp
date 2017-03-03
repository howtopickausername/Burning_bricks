#include "stdafx.h"
#include "State.h"


cState::cState(pStateMap map)
	:StateMap(map)
{
	StateBreed = std::make_shared<cStateBreed>(StateMap->Map.at(0));
}

cState::~cState()
{

}

void cState::Update()
{
	StateBreed->fUpdate();
}

void cState::HandleInput(pCommand cmd)
{
	auto rt = StateBreed->fHandleInput(cmd);
	if (std::get<0>(rt) == cStateBreed::OP::eRoleBack){
		auto old = StateStack.top();
		StateStack.pop();
		StateBreed->fPost();
		StateBreed = old;
		old->fPre();
	}
	else if (std::get<0>(rt) == cStateBreed::OP::eNew) {
		auto newBreed = StateMap->Map.find(std::get<1>(rt));
		if (newBreed == StateMap->Map.end()){
			newBreed = StateMap->ParentMap->find(std::get<1>(rt));
		}
		StateBreed->fPost();
		StateStack.push(StateBreed);
		StateBreed = std::make_shared<cStateBreed>(newBreed->second);
		StateBreed->fPre();
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