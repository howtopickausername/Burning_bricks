#pragma once
#include <memory>
#include <string>

class cModelCtr
{
public:
	cModelCtr();
	virtual ~cModelCtr();

	std::wstring resName;
	float keyTime;	//动画类型需要
};
typedef std::shared_ptr<cModelCtr> pModelCtr;