#include "stdafx.h"
#include <iostream>
#include "MonsterFactory.h"


cMonsterFactory::cMonsterFactory()
{

}

cMonsterFactory::~cMonsterFactory()
{

}

pGameObject cMonsterFactory::CreateMonster()
{
	pStateMap monsterMap = std::make_shared<cStateMap>();
	cStateBreed sb;
	sb.fUpdate = std::function<void(void)>([]() {std::cout << "is Idling" << std::endl; });
	sb.fHandleInput = std::function<std::tuple<cStateBreed::OP, int>(pCommand cmd)>([](pCommand cmd) {
		return std::make_tuple(cStateBreed::OP::eConstant, 0);
	});
	monsterMap->Map.insert(std::make_pair(0, sb));
	pState monsterState = std::make_shared<cState>(monsterMap);
	pGameObject monster = std::make_shared<cGameObject>(*monsterState.get());
	return monster;
}
