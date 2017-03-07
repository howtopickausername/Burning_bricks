#pragma once
#include <memory>

class cCanvas
{
public:
	cCanvas();
	~cCanvas();

	virtual void begin();
	virtual void DrawRect(float left, float top, float right, float bottom, int color);
	virtual void end();
private:

};
typedef std::shared_ptr<cCanvas> pCanvas;