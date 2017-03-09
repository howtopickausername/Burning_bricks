#include "stdafx.h"
#include "KetboardCmdProductor.h"

cKbCmdProductor::cKbCmdProductor()
{
	cCmdBase cmd(eNone);
	CmdUpMaps.fill(cmd);
	CmdDownMaps.fill(cmd);
}

void cKbCmdProductor::KeyUp(int key)
{
	assert(0 <= key && key < cKeyboard::NumOfKeys);
	if (CmdUpMaps[key].ID != eNone) {
		CmdQ.push(std::make_shared<cCmdBase>(CmdUpMaps[key]));
	}
}

void cKbCmdProductor::KeyDown(int key)
{

}

void cKbCmdProductor::KeyDownOnce(int key)
{
	assert(0 <= key && key < cKeyboard::NumOfKeys);
	if (CmdDownMaps[key].ID != eNone) {
		CmdQ.push(std::make_shared<cCmdBase>(CmdDownMaps[key]));
	}
}
