#pragma once
#include <string>
#include <vector>
#include <functional>
#include <tuple>
#include <memory>
#include "TypesOfProperty.h"
#include "Game/Command.h"

class cStateBreed
{
public:
	enum class OP {
		eConstant,
		eRoleBack,
		eNew
	};
	cStateBreed();
	~cStateBreed();

	std::function<void(void)> fPre;	
	std::function<void(void)> fPost;
	std::function<void(void)> fUpdate;
	std::function<std::tuple<OP, int>(pCommand cmd)> fHandleInput;
	//数据段的初始化应该在fPre里面做
	std::vector<uPropValue> Buf;
	std::vector<std::string> StrBuf;
};
typedef std::shared_ptr<cStateBreed> pStateBreed;