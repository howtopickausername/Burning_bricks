#include "stdafx.h"
#include <iostream>
#include "MonsterFactory.h"


cMonsterFactory::cMonsterFactory() {

}

cMonsterFactory::~cMonsterFactory() {

}

pGameObject cMonsterFactory::CreateMonster() {
	pStateMap monsterMap = std::make_shared<cStateMap>();
	cStateBreed sb;
	sb.fUpdate = std::function<void(cGameObject& obj)>([](cGameObject&) {std::cout << "is Idling" << std::endl; });
	sb.fHandleInput = std::function<std::tuple<cStateBreed::OP, int>(cGameObject&, pCommand cmd)>([](cGameObject& obj, pCommand cmd) {
		OutputDebugString(L"fHandleInput");
		switch (cmd->ID) {
		case eCmd::eJump:
		{
			cGameObject::pProp & bProps = obj.GetBackProps();
			auto bhp = bProps.find(ePropTypes::eHp);
			if (bhp != bProps.end()) {
				bhp->second.fv *= 0.8;
				OutputDebugString(L"eJump");
			}
		}
		break;
		default:
			break;
		}
		return std::make_tuple(cStateBreed::OP::eConstant, 0);
	});
	monsterMap->Map.insert(std::make_pair(0, sb));
	pState monsterState = std::make_shared<cState>(monsterMap);
	pGameObject monster = std::make_shared<cGameObject>(*monsterState.get());
	return monster;
}
