#pragma once
#include <memory>

class cModelColor
{
public:
	cModelColor();
	~cModelColor();

	unsigned int PSId;	//������ɫ��
};
typedef std::shared_ptr<cModelColor> pModelColor;