#pragma once
#include <memory>

class cModel
{
public:
	cModel();
	virtual ~cModel();

	unsigned int resId;
	float keyTime;	//����������Ҫ
};
typedef std::shared_ptr<cModel> pModel;