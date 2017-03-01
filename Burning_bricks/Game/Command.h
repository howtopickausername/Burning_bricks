#pragma once
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
};