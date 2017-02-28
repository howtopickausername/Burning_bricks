#pragma once
#include <stack>
#include "StateMap.h"

class cState
{
public:
	cState(pStateMap map);
	~cState();
	void Update();
	void HandleInput(cCommand const& cmd);
	std::shared_ptr<cState> Copy() const;

	pStateMap StateMap;
	pStateBreed StateBreed;
	std::stack<pStateBreed> StateStack;
};
typedef std::shared_ptr<cState> pState;