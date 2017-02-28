#pragma once
#include <queue>
#include "State.h"

class cGameObject
{
public:
	cGameObject(cState const& state);
	~cGameObject();
	void SendCmd(cCommand const& cmd);
	void HandleInput();
	void Update();
	void Swap();
	std::shared_ptr<cGameObject> Copy();

private:
	std::queue<cCommand> CurCmdQ;
	std::queue<cCommand> BackCmdQ;
	pState CurState;
	pState BackState;
};
typedef std::shared_ptr<cGameObject> pGameObject;