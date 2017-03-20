#include "stdafx.h"
#include <iostream>
#include "MonsterFactory.h"
#include "Locator.h"
#include "DirectXTK/Inc/SimpleMath.h"


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
		sb.Buf.resize(1);
		sb.Buf[0].iv = 0;
		auto& models = obj.GetModels();
		//models[0].model.resName = L"1";	//站立
		models[0].model = cLocator::ModelRes().GetModel(L"Nanosuit2\\nanosuit2.3ds");
	});
	stand.fUpdate = cStateBreed::fUpdateType([](cStateBreed& sb, cGameObject& obj) {
		//auto& models = obj.GetModels();
		//float s = std::sin(1.0f * sb.Buf[0].iv / FramesRate::num / 2);
		//using namespace DirectX::SimpleMath;
		//Matrix trans;
		//trans *= 0.3;
		//trans = Matrix::Transform(trans, Quaternion::CreateFromAxisAngle(Vector3::Up, s * DirectX::XM_PI));
		//trans.Translation(Vector3::UnitY * -5);
		//trans.Translation(Vector3::UnitZ * -2);
		//models[0].transform = trans;
		/*auto bhp = obj.GetBackProps().find(ePropTypes::eHp);
		if (bhp != obj.GetBackProps().end()) {

			bhp->second.fv += 1.0f / FramesRate::num / 2;
			if (bhp->second.fv > 1) {
				return cSbRtn(SbOp::eNew, eWalk);
			}
		}*/
		sb.Buf[0].iv++;
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
		//models[0].model.resName = L"2";	//行走
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
		//obj.GetModels()[0].model.resName = L"3";
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

pGameObject cMonsterFactory::CreateCamera()
{
	pStateMap monsterMap = std::make_shared<cStateMap>();
	cStateBreed sb(0);
	sb.fUpdate = cStateBreed::fUpdateType([](cStateBreed& sb, cGameObject& obj) {
		return cSbRtn(SbOp::eConstant, eNone);
	});
	sb.fHandleInput = cStateBreed::fHdType([](cStateBreed& sb, cGameObject& obj, pCommand cmd) {
		if (cmd->ID == eMoveWorld){
			std::shared_ptr<cCmdMoveW> scmd = std::dynamic_pointer_cast<cCmdMoveW>(cmd);
			char axi = scmd->At<0>();
			float speed = scmd->At<1>();
			using namespace DirectX::SimpleMath;
			Vector3 dir;
			if (axi == 1){
				dir = Vector3::UnitX;
			}
			else if (axi == 2){
				dir = Vector3::UnitY;
			}
			else {
				dir = Vector3::UnitZ;
			}
			auto& cameras = cLocator::Graphics().GetCameras();
			cameras[0].pos = cameras[0].pos + dir * speed / FramesRate::num;
		}
		else if(cmd->ID == eMouseMove){
			std::shared_ptr<cCmdMouseMove> scmd = std::dynamic_pointer_cast<cCmdMouseMove>(cmd);
			int dx, dy;
			std::tie(dx, dy) = scmd->Param;
			float rateX = 1.0f * dx / cLocator::Graphics().Width() * DirectX::XM_PI;
			float rateY = 1.0f * dy / cLocator::Graphics().Height() * DirectX::XM_PI;
			auto& cameras = cLocator::Graphics().GetCameras();
			using namespace DirectX::SimpleMath;
			Vector3 right = cameras[0].up;
			right.Cross(cameras[0].dir);
			right.Normalize();
			cameras[0].dir = Vector3::Transform(cameras[0].dir, Matrix::CreateFromAxisAngle(cameras[0].up, rateX));
			cameras[0].dir = Vector3::Transform(cameras[0].dir, Matrix::CreateFromAxisAngle(right, rateY));
		}
		return cSbRtn(SbOp::eConstant, eNone);
	});
	monsterMap->Insert(sb);
	pState state = std::make_shared<cState>(monsterMap);
	return std::make_shared<cGameObject>(2, *state.get());
}
