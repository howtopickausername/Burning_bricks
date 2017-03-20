#pragma once
#include <memory>
#include <array>
#include <queue>
#include "Controler/Mouse.h"
#include "Command.h"

class cMouseCmdProductor : public iMouseReceiver
{
public:
	cMouseCmdProductor();

	virtual void MouseMoved(int dx, int dy) override;
	virtual void MouseButtonDown(int button) override;
	virtual void MouseButtonUp(int button) override;

	std::array<std::shared_ptr<cCmdBase>, cMouse::NumOfBtns> CmdDownMap;
	std::array<std::shared_ptr<cCmdBase>, cMouse::NumOfBtns> CmdUpMap;
	std::shared_ptr<cCmdMouseMove> CmdMove;
	std::queue<pCommand> CmdQ;
};
typedef std::shared_ptr<cMouseCmdProductor> pMsCmdProductor;