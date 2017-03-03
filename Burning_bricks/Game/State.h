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
	const pStateBreed GetBreed() const;
	std::shared_ptr<cState> Copy() const;

private:
	pStateMap StateMap;
	pStateBreed StateBreed;
	std::stack<pStateBreed> StateStack;
};
typedef std::shared_ptr<cState> pState;