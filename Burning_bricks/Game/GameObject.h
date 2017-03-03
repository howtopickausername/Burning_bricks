#pragma once
#include <vector>
#include <queue>
#include "State.h"

class cGameObject
{
public:
	cGameObject(cState const& basic_state);
	~cGameObject();
	void SendCmd(pCommand cmd);
	void ProcessCmd();
	void Update();
	void Swap();
	//基础状态保存所有通用属性,位置、方向、类型
	const pState GetCommonState() const;
	std::shared_ptr<cGameObject> Copy();

private:
	std::queue<pCommand> CurCmdQ;
	std::queue<pCommand> BackCmdQ;
	std::vector<pState> CurState;
	std::vector<pState> BackState;
};
typedef std::shared_ptr<cGameObject> pGameObject;