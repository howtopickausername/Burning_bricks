#include "stdafx.h"
#include "StateBreed.h"

cStateBreed::cStateBreed()
{
	fPre = std::function<void(cGameObject&)>([](cGameObject&) {; });
	fPost = std::function<void(cGameObject&)>([](cGameObject&) {; });
	fUpdate = std::function<void(cGameObject&)>([](cGameObject&) {; });
	fHandleInput = std::function<std::tuple<OP, int>(cGameObject&, pCommand cmd)>([](cGameObject&, pCommand) { return std::make_tuple(OP::eConstant, 0); });
}

cStateBreed::~cStateBreed()
{

}
