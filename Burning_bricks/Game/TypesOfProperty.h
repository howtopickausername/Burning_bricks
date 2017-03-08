#pragma once

enum class ePropTypes {
	eType,
	eAlive,
	eHp,
	eCustom,
	ePosX,
	ePosY,
	ePosZ,
	eDirX,
	eDirY,
	eDirZ
};

union cPropValue
{
	int iv;
	float fv;
};

inline cPropValue MakePropValue(int value) {
	cPropValue v;
	v.iv = value;
	return v;
}

inline cPropValue MakePropValue(float value) {
	cPropValue v;
	v.fv = value;
	return v;
}