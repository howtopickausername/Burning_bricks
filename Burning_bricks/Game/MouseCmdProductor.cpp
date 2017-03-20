#include "stdafx.h"
#include "MouseCmdProductor.h"


cMouseCmdProductor::cMouseCmdProductor()
{
	std::shared_ptr<cCmdBase> cmd = std::make_shared<cCmdBase>();
	CmdUpMap.fill(cmd);
	CmdDownMap.fill(cmd);
	CmdMove = std::make_shared<cCmdMouseMove>(0,0);
}

void cMouseCmdProductor::MouseMoved(int dx, int dy)
{
	CmdMove->At<0>() = dx;
	CmdMove->At<1>() = dy;
	CmdQ.push(CmdMove);
}

void cMouseCmdProductor::MouseButtonDown(int button)
{
	assert(0 <= button && button < cMouse::NumOfBtns);
	if (CmdDownMap[button]->ID != eNone) {
		CmdQ.push(CmdDownMap[button]);
	}
}

void cMouseCmdProductor::MouseButtonUp(int button)
{
	assert(0 <= button && button < cMouse::NumOfBtns);
	if (CmdUpMap[button]->ID != eNone) {
		CmdQ.push(CmdUpMap[button]);
	}
}
