#pragma once
#include <string>
#include <vector>
#include <functional>
#include <tuple>
#include <memory>
#include "TypesOfProperty.h"
#include "Game/Command.h"

class cGameObject;

enum class SbOp {
	eConstant,
	eRoleBack,
	eNew
};
class cSbRtn {
public:
	cSbRtn(SbOp op, int n);
	SbOp operate;
	int id;
};
typedef std::shared_ptr<cSbRtn> pSbRtn;
class cStateBreed
{
public:
	typedef unsigned int IdType;
	typedef std::function<void(cStateBreed&, cGameObject&)> fPreType;	//pre pos ���ܸı�״̬
	typedef fPreType fPostType;
	typedef std::function<cSbRtn(cStateBreed&, cGameObject&)> fUpdateType;
	typedef std::function<cSbRtn(cStateBreed&, cGameObject&, pCommand cmd)> fHdType;
	cStateBreed(IdType id);
	~cStateBreed();
	const IdType Id;
	fPreType fPre;	
	fPostType fPost;
	fUpdateType fUpdate;
	fHdType fHandleInput;
	//���ݶεĳ�ʼ��Ӧ����fPre������
	std::vector<cPropValue> Buf;
};
typedef std::shared_ptr<cStateBreed> pStateBreed;