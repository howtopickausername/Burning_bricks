#pragma once
#include <string>
#include <vector>
#include <memory>
//#include "CommandBreed.h"

enum eCmd {
	eNone = 0,
	eGoForward,
	eRetreat,
	eTurnLeft,
	eTurnRight,
	eTurnBack,
	eJump
};

class cCommand
{
public:
	cCommand(eCmd id = eNone);
	~cCommand();
	eCmd ID;
	std::vector<int> IntP;
	std::vector<std::string> StrP;
};
typedef std::shared_ptr<cCommand> pCommand;