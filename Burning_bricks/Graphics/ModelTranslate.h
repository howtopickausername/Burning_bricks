#pragma once
#include <memory>

class cModelTrans
{
public:
	cModelTrans();
	~cModelTrans();

	unsigned int VSId;	//������ɫ��
};
typedef std::shared_ptr<cModelTrans> pModelTrans;