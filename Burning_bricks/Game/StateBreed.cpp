#include "stdafx.h"
#include "StateBreed.h"

cSbRtn::cSbRtn(SbOp op, int n)
	:operate(op), id(n)
{

}

cStateBreed::cStateBreed(IdType id)
	:Id(id)
{
	fPre = fPreType([](cStateBreed&, cGameObject&) {; });
	fPost = fPreType([](cStateBreed&, cGameObject&) {; });
	fUpdate = fUpdateType([](cStateBreed&, cGameObject&) { return cSbRtn(SbOp::eConstant, 0); });
	fHandleInput = fHdType([](cStateBreed&, cGameObject&, pCommand) { return cSbRtn(SbOp::eConstant, 0); });
}

cStateBreed::~cStateBreed()
{

}
