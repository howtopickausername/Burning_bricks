#pragma once
#include <memory>
#include <string>

class cModelCtr
{
public:
	cModelCtr();
	virtual ~cModelCtr();

	std::wstring resName;
	float keyTime;	//����������Ҫ
};
typedef std::shared_ptr<cModelCtr> pModelCtr;