#include "stdafx.h"
#include <iostream>
#include "MonsterFactory.h"


cMonsterFactory::cMonsterFactory() {

}

cMonsterFactory::~cMonsterFactory() {

}

pGameObject cMonsterFactory::CreateMonster() {
	enum ebs
	{
		eNone,
		eStand,
		eWalk,
		eSkill
	};
	pStateMap monsterMap = std::make_shared<cStateMap>();
	cStateBreed sb0(eNone);	//初始化
	sb0.fPre = cStateBreed::fPreType([](cStateBreed& sb, cGameObject& obj) {
		auto& bhp = obj.GetBackProps();
		bhp[ePropTypes::eHp] = MakePropValue(1.0f);
		bhp[ePropTypes::eCustom] = MakePropValue(0.0f);
	});
	sb0.fUpdate = cStateBreed::fUpdateType([](cStateBreed& sb, cGameObject& obj) {
		return cSbRtn(SbOp::eNew, eStand);
	});
	monsterMap->Insert(sb0);
	cStateBreed stand(eStand);	//站立 0
	stand.fPre = cStateBreed::fPreType([](cStateBreed& sb, cGameObject& obj) {
		auto& models = obj.GetModels();
		models[0].model.resId = 1;	//站立
	});
	stand.fUpdate = cStateBreed::fUpdateType([](cStateBreed& sb, cGameObject& obj) {
		auto bhp = obj.GetBackProps().find(ePropTypes::eHp);
		if (bhp != obj.GetBackProps().end()) {
			bhp->second.fv += 1.0f / FramesRate::num / 2;
			if (bhp->second.fv > 1) {
				return cSbRtn(SbOp::eNew, eWalk);
			}
		}
		return cSbRtn(SbOp::eConstant, eNone);
	});
	stand.fHandleInput = cStateBreed::fHdType([](cStateBreed& sb, cGameObject& obj, pCommand cmd) {
		if (cmd->ID == eCmd::eGoForward) {
			return cSbRtn(SbOp::eNew, eWalk);
		}
		else if (cmd->ID == eCmd::eSkill) {
			return cSbRtn(SbOp::eNew, eSkill);
		}
		return cSbRtn(SbOp::eConstant, eNone);
	});
	monsterMap->Insert(stand);
	cStateBreed walk(eWalk); //行走 1
	walk.fPre = cStateBreed::fPreType([](cStateBreed& sb, cGameObject& obj) {
		auto& models = obj.GetModels();
		models[0].model.resId = 2;	//行走
	});
	walk.fUpdate = cStateBreed::fUpdateType([](cStateBreed& sb, cGameObject& obj) {
		auto bhp = obj.GetBackProps().find(ePropTypes::eHp);
		if (bhp != obj.GetBackProps().end()){
			bhp->second.fv -= 1.0f / FramesRate::num / 2;
			if (bhp->second.fv < 0) {
				return cSbRtn(SbOp::eNew, eStand);
			}
		}
		return cSbRtn(SbOp::eConstant, eNone);
	});
	walk.fHandleInput = cStateBreed::fHdType([](cStateBreed& sb, cGameObject& obj, pCommand cmd) {
		if (cmd->ID == eCmd::eTurnBack) {
			return cSbRtn(SbOp::eNew, eStand);
		}
		else if (cmd->ID == eCmd::eSkill) {
			return cSbRtn(SbOp::eNew, eSkill);
		}
		return cSbRtn(SbOp::eConstant, eNone);
	});
	monsterMap->Insert(walk);
	cStateBreed skill(eSkill);
	skill.fPre = cStateBreed::fPreType([](cStateBreed& sb, cGameObject& obj) {
		sb.Buf.resize(1);
		sb.Buf[0] = MakePropValue(0.0f);
		obj.GetModels()[0].model.resId = 3;
	});
	skill.fUpdate = cStateBreed::fUpdateType([](cStateBreed& sb, cGameObject& obj) {
		auto& Buf = sb.Buf;
		Buf[0].iv++;
		if (Buf[0].iv >= 2 * FramesRate::num){
			return cSbRtn(SbOp::eRoleBack, eNone);
		}
		auto& prop = obj.GetBackProps();
		prop[ePropTypes::eCustom] = MakePropValue(static_cast<float>(Buf[0].iv) / (2 * FramesRate::num));
		return cSbRtn(SbOp::eConstant, eNone);
	});
	skill.fHandleInput = cStateBreed::fHdType([](cStateBreed& sb, cGameObject& obj, pCommand cmd) {
		if (cmd->ID == eCmd::eSkill) {
			return cSbRtn(SbOp::eRoleBack, eNone);
		}
		return cSbRtn(SbOp::eConstant, eNone);
	});
	monsterMap->Insert(skill);
	pState monsterState = std::make_shared<cState>(monsterMap);
	pGameObject monster = std::make_shared<cGameObject>(1, *monsterState.get());
	return monster;
}
