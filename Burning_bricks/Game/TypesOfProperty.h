#pragma once

enum class ePropTypes {
	eType,
	eAlive,
	eHp,
	ePosX,
	ePosY,
	ePosZ,
	eDirX,
	eDirY,
	eDirZ
};

union uPropValue
{
	int iv;
	float fv;
};