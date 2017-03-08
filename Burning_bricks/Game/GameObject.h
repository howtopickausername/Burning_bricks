#pragma once
#include <array>
#include <vector>
#include <map>
#include <queue>
#include "Command.h"
#include "TypesOfProperty.h"
#include "State.h"
#include "Graphics/Canvas.h"
#include "Graphics/Model.h"
#include "Graphics/ModelColor.h"
#include "Graphics/ModelTranslate.h"

class cModelInfo {
public:
	cModel model;
	cModelColor color;
	cModelTrans trans;
};

class cGameObject
{
public:
	friend cStateBreed;
	typedef std::map<ePropTypes, cPropValue> pProp;
	typedef unsigned int IdType;
	cGameObject(IdType id, cState const& basic_state);
	~cGameObject();
	void SendCmd(pCommand cmd);
	void ProcessCmd();
	void Update();
	void Swap();
	void Render(pCanvas & canvas);
	//基础信息保存位置、方向、类型等
	pProp const& GetProps() const;
	pProp & GetBackProps();
	std::array<cModelInfo, 3>& GetModels();
	std::shared_ptr<cGameObject> Copy();
	const IdType Id;

private:
	std::array<cModelInfo, 3> Models;
	std::map<ePropTypes, cPropValue> Props;
	std::map<ePropTypes, cPropValue> BackProps;
	std::queue<pCommand> CmdQ;
	std::vector<pState> State;
};
typedef std::shared_ptr<cGameObject> pGameObject;