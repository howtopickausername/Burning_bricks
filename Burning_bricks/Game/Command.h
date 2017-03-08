#pragma once
#include <string>
#include <vector>
#include <memory>
//#include "CommandBreed.h"

enum eCmd {
	eNone = 0,
	eCustom,
	eGoForward,
	eRetreat,
	eTurnLeft,
	eTurnRight,
	eTurnBack,
	eJump,
	eSkill,
};

class cCommand
{
public:
	explicit cCommand(eCmd id = eNone);
	~cCommand();
	eCmd ID;
	int Param;
};
typedef std::shared_ptr<cCommand> pCommand;