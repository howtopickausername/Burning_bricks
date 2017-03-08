#include "stdafx.h"
#include <iostream>
#include "MonsterFactory.h"


cMonsterFactory::cMonsterFactory() {

}

cMonsterFactory::~cMonsterFactory() {

}

pGameObject cMonsterFactory::CreateMonster() {
	pStateMap monsterMap = std::make_shared<cStateMap>();
	cStateBreed sb0(0);	//初始化
	sb0.fPre = std::function<void(cGameObject& obj)>([](cGameObject& obj) {
		auto& bhp = obj.GetBackProps();
		uPropValue hp;
		hp.fv = 1.0f;
		bhp[ePropTypes::eHp] = hp;
		pCommand cmd = std::make_shared<cCommand>(eCmd::eCustom);
		obj.SendCmd(cmd);
	});
	sb0.fHandleInput = std::function<std::tuple<cStateBreed::OP, int>(cGameObject&, pCommand cmd)>([](cGameObject& obj, pCommand cmd) {
		return std::make_tuple(cStateBreed::OP::eNew, 1);
	});
	monsterMap->Insert(sb0);
	cStateBreed sb1(1);	//站立 0
	sb1.fPre = std::function<void(cGameObject& obj)>([](cGameObject& obj) {
		auto& models = obj.GetModels();
		models[0].model.resId = 1;	//站立
	});
	sb1.fUpdate = std::function<void(cGameObject& obj)>([](cGameObject& obj) {
		auto bhp = obj.GetBackProps().find(ePropTypes::eHp);
		if (bhp != obj.GetBackProps().end()) {
			bhp->second.fv += 1.0f / FramesRate::num / 2;
			if (bhp->second.fv > 1) {
				pCommand cmd = std::make_shared<cCommand>(eCmd::eCustom);
				obj.SendCmd(cmd);
			}
		}
	});
	sb1.fHandleInput = std::function<std::tuple<cStateBreed::OP, int>(cGameObject&, pCommand cmd)>([](cGameObject& obj, pCommand cmd) {
		if (cmd->ID == eCmd::eGoForward) {
			return std::make_tuple(cStateBreed::OP::eNew, 2);
		}
		else if (cmd->ID == eCmd::eCustom) {
			return std::make_tuple(cStateBreed::OP::eNew, 2);
		}
		return std::make_tuple(cStateBreed::OP::eConstant, 0);
	});
	monsterMap->Insert(sb1);
	cStateBreed sb2(2); //行走 1
	sb2.fPre = std::function<void(cGameObject& obj)>([](cGameObject& obj) {
		auto& models = obj.GetModels();
		models[0].model.resId = 2;	//行走
	});
	sb2.fUpdate = std::function<void(cGameObject& obj)>([](cGameObject& obj) {
		auto bhp = obj.GetBackProps().find(ePropTypes::eHp);
		if (bhp != obj.GetBackProps().end()){
			bhp->second.fv -= 1.0f / FramesRate::num / 2;
			if (bhp->second.fv < 0) {
				pCommand cmd = std::make_shared<cCommand>(eCmd::eCustom);
				obj.SendCmd(cmd);
			}
		}
	});
	sb2.fHandleInput = std::function<std::tuple<cStateBreed::OP, int>(cGameObject&, pCommand cmd)>([](cGameObject& obj, pCommand cmd) {
		if (cmd->ID == eCmd::eTurnBack) {
			return std::make_tuple(cStateBreed::OP::eNew, 1);
		}
		else if (cmd->ID == eCmd::eCustom) {
			return std::make_tuple(cStateBreed::OP::eNew, 1);
		}
		return std::make_tuple(cStateBreed::OP::eConstant, 0);
	});
	monsterMap->Insert(sb2);
	pState monsterState = std::make_shared<cState>(monsterMap);
	pGameObject monster = std::make_shared<cGameObject>(1, *monsterState.get());
	return monster;
}
