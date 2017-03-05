#pragma once
#include <memory>

class cModelColor
{
public:
	cModelColor();
	~cModelColor();

	unsigned int PSId;	//ÏñËØ×ÅÉ«Æ÷
};
typedef std::shared_ptr<cModelColor> pModelColor;