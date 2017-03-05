#pragma once
#include <memory>

class cModel
{
public:
	cModel();
	virtual ~cModel();

	unsigned int resId;
	float keyTime;	//动画类型需要
};
typedef std::shared_ptr<cModel> pModel;