#include "stdafx.h"
#include "KetboardCmdProductor.h"

cKbCmdProductor::cKbCmdProductor()
{
	std::shared_ptr<cCmdBase> cmd = std::make_shared<cCmdBase>();
	CmdUpMaps.fill(cmd);
	CmdDownMaps.fill(cmd);
}

void cKbCmdProductor::KeyUp(int key)
{
	assert(0 <= key && key < cKeyboard::NumOfKeys);
	if (CmdUpMaps[key]->ID != eNone) {
		CmdQ.push(CmdUpMaps[key]);
	}
}

void cKbCmdProductor::KeyDown(int key)
{
	if (CmdDownMaps[key]->ID != eNone) {
		CmdQ.push(CmdDownMaps[key]);
	}
}

void cKbCmdProductor::KeyDownOnce(int key)
{
	assert(0 <= key && key < cKeyboard::NumOfKeys);
// 	if (CmdDownMaps[key]->ID != eNone) {
// 		CmdQ.push(CmdDownMaps[key]);
// 	}
}
