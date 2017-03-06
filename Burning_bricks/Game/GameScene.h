#pragma once
#include <map>
#include "Locator.h"
#include "GameObject.h"

class cGameScene
{
public:
	cGameScene();
	~cGameScene();
	void CreateScene();
	void DestroyScene();
	void DispatchCmd(pCommand cmd);
	void ProcessCmd();
	void Update();
	void Render(pCanvas & canvas);

protected:
	void Swap();

	cLocator& Locator();
	pGameObject CurObj;
	std::map<int, pGameObject> Objs;

private:
	static cLocator* m_Locator;
};
typedef std::shared_ptr<cGameScene> pGameScene;