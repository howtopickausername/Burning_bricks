#pragma once
#include <string>
#include <vector>
#include <memory>

enum eCmd {
	eNone = 0,
	eMouseMove,
	eCustom,
	eGoForward,
	eRetreat,
	eTurnLeft,
	eTurnRight,
	eTurnBack,
	eJump,
	eSkill,
	eMoveWorld,	//按照世界坐标轴位移对象
	eMoveLocal,	//按照对象本地坐标轴
};

class cCmdBase
{
public:
	cCmdBase(eCmd id = eNone);
	virtual ~cCmdBase();
	eCmd ID;
};
typedef std::shared_ptr<cCmdBase> pCommand;

template <eCmd id, class... _Types>
class cCommand : public cCmdBase
{
public:
	cCommand(_Types...arg) :cCmdBase(id), Param(arg...) {};
	virtual ~cCommand() {};
	template<size_t pos>
	auto At()->decltype(std::get<pos>(Param)) {
		return std::get<pos>(Param);
	}
	std::tuple<_Types...> Param;
};

typedef cCommand<eMouseMove, int, int> cCmdMouseMove;
typedef cCommand<eMoveWorld, char, float> cCmdMoveW;
typedef cCommand<eMoveLocal, char, float> cCmdMoveL;