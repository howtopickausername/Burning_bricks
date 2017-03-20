#pragma once
#include "GameObject.h"

class cMonsterFactory
{
public:
	cMonsterFactory();
	~cMonsterFactory();

	static pGameObject CreateMonster();
	static pGameObject CreateCamera();
};