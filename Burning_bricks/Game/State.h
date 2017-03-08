#pragma once
#include <stack>
#include "StateMap.h"

class cGameObject;
class cState
{
public:
	cState(pStateMap map);
	~cState();
	void Init(cGameObject& obj);
	void Update(cGameObject& obj);
	void HandleInput(cGameObject& obj, pCommand cmd);
	const pStateBreed GetBreed() const;
	std::shared_ptr<cState> Copy() const;

private:
	pStateMap StateMap;
	pStateBreed StateBreed;
	std::stack<cStateBreed::IdType> StateStack;
};
typedef std::shared_ptr<cState> pState;