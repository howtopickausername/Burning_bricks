#pragma once
#include <string>
#include <memory>

class cCanvas
{
public:
	cCanvas();
	~cCanvas();

	virtual void begin();
	virtual void DrawRect(float left, float top, float right, float bottom, int color);
	virtual void DrawText(float left, float top, float right, float bottom, std::wstring text);
	virtual void end();
private:

};
typedef std::shared_ptr<cCanvas> pCanvas;