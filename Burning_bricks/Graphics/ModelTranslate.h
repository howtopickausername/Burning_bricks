#pragma once
#include <memory>

class cModelTrans
{
public:
	cModelTrans();
	~cModelTrans();

	unsigned int VSId;	//¶¥µã×ÅÉ«Æ÷
};
typedef std::shared_ptr<cModelTrans> pModelTrans;