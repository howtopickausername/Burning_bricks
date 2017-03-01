#pragma once
#include <memory>
#include <array>
#include <queue>
#include "Controler\Keyboard.h"
#include "Command.h"

class cKbCmdProductor : public iKeyboadrReceiver
{
public:
	cKbCmdProductor();

	virtual void KeyUp(int key) override;
	virtual void KeyDown(int key) override;
	virtual void KeyDownOnce(int key) override;

	std::array<cCommand, cKeyboard::NumOfKeys> CmdUpMaps;
	std::array<cCommand, cKeyboard::NumOfKeys> CmdDownMaps;
	std::queue<cCommand> CmdQ;
};
typedef std::shared_ptr<cKbCmdProductor> pKbCmdproductor;