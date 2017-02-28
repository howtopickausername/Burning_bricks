#pragma once
#include <functional>
#include <tuple>
#include <memory>
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
	std::function<std::tuple<OP, int>(cCommand const & cmd)> fHandleInput;
};
typedef std::shared_ptr<cStateBreed> pStateBreed;