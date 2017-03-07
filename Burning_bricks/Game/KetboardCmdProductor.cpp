#include "stdafx.h"
#include "KetboardCmdProductor.h"

cKbCmdProductor::cKbCmdProductor()
{
}

void cKbCmdProductor::KeyUp(int key)
{
	assert(0 <= key && key < cKeyboard::NumOfKeys);
	if (CmdUpMaps[key]) {
		CmdQ.push(CmdUpMaps[key]);
	}
}

void cKbCmdProductor::KeyDown(int key)
{

}

void cKbCmdProductor::KeyDownOnce(int key)
{
	assert(0 <= key && key < cKeyboard::NumOfKeys);
	if (CmdDownMaps[key]) {
		CmdQ.push(CmdDownMaps[key]);
	}
}
