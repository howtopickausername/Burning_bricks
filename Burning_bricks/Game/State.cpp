#include "stdafx.h"
#include "State.h"


cState::cState(pStateMap map)
	:StateMap(map)
{
	StateBreed = std::make_shared<cStateBreed>(StateMap->At(0));
}

cState::cState(cState const & right) {
	StateMap = right.StateMap;
	StateBreed = right.StateBreed;
	StateStack = right.StateStack;
	NewBreed = right.NewBreed;
}

cState::cState(cState && right) {
	StateMap = std::move(right.StateMap);
	StateBreed = std::move(right.StateBreed);
	StateStack = std::move(right.StateStack);
	NewBreed = std::move(right.NewBreed);
}

cState & cState::operator=(cState && right) {
	StateMap = std::move(right.StateMap);
	StateBreed = std::move(right.StateBreed);
	StateStack = std::move(right.StateStack);
	NewBreed = std::move(right.NewBreed);
	return *this;
}

cState::~cState()
{

}

void cState::Init(cGameObject & obj) {
	StateBreed->fPre(*(StateBreed.get()), obj);
}

void cState::Update(cGameObject& obj)
{
	if (CheckNewBreed(obj)) {
		return;
	}
	auto rt = StateBreed->fUpdate(*(StateBreed.get()), obj);
	if (rt.operate == SbOp::eNew || rt.operate == SbOp::eRoleBack){
		NewBreed = std::make_shared<cSbRtn>(std::move(rt));
	}
}

void cState::HandleInput(cGameObject& obj, pCommand cmd)
{
	if (CheckNewBreed(obj)){
		return;
	}
	auto rt = StateBreed->fHandleInput(*(StateBreed.get()), obj, cmd);
	if (rt.operate == SbOp::eNew || rt.operate == SbOp::eRoleBack) {
		NewBreed = std::make_shared<cSbRtn>(std::move(rt));
	}
}

const pStateBreed cState::GetBreed() const {
	return StateBreed;
}

std::shared_ptr<cState> cState::Copy() const
{
	return std::make_shared<cState>(*this);
}

bool cState::CheckNewBreed(cGameObject& obj)
{
	//首先判断本次是否要更新到新状态
	if (NewBreed) {
		StateBreed->fPost(*(StateBreed.get()), obj);
		if (NewBreed->operate == SbOp::eNew) {	//切换到新状态
			StateStack.push(StateBreed->Id);
			StateBreed = std::make_shared<cStateBreed>(StateMap->At(NewBreed->id));
		}
		else if (NewBreed->operate == SbOp::eRoleBack) {	//回滚
			StateBreed = std::make_shared<cStateBreed>(StateMap->At(StateStack.top()));
			StateStack.pop();
		}
		StateBreed->fPre(*(StateBreed.get()), obj);
		NewBreed.reset();
		return true;
	}
	return false;
}
