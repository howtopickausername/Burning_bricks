#pragma once
#include <string>
#include <vector>
#include <memory>
//#include "CommandBreed.h"

enum eCmd {
	eNone = 0,
	eCustom,
	eGoForward,
	eRetreat,
	eTurnLeft,
	eTurnRight,
	eTurnBack,
	eJump,
	eSkill,
};

class cCmdBase
{
public:
	explicit cCmdBase(eCmd id = eNone);
	~cCmdBase();
	eCmd ID;
};
typedef std::shared_ptr<cCmdBase> pCommand;

template <class... _Types>
class cCommand : public cCmdBase
{
public:
	template<size_t pos>
	auto Get()->decltype(std::get<pos>(Param)) {
		return std::get<pos>(Param);
	}
	std::tuple<_Types...> Param;
};