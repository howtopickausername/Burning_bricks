#include "stdafx.h"
#include "StateBreed.h"

cStateBreed::cStateBreed()
{
	fPre = std::function<void(void)>([]() {; });
	fPost = std::function<void(void)>([]() {; });
	fUpdate = std::function<void(void)>([]() {; });
	fHandleInput = std::function<std::tuple<OP, int>(pCommand cmd)>([](pCommand) { return std::make_tuple(OP::eConstant, 0); });
}

cStateBreed::~cStateBreed()
{

}
