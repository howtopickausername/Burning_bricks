#include "stdafx.h"
#include "GameScene.h"
#include "MonsterFactory.h"

cLocator* cGameScene::m_Locator = nullptr;

cGameScene::cGameScene()
{

}

cGameScene::~cGameScene()
{

}

void cGameScene::CreateScene()
{
	pGameObject camera = cMonsterFactory::CreateCamera();
	Objs.insert(std::make_pair(camera->Id, camera));
	pGameObject monster = cMonsterFactory::CreateMonster();
	Objs.insert(std::make_pair(monster->Id, monster));
	CurObj = camera;
}

void cGameScene::DestroyScene()
{

}

void cGameScene::ProcessCmd()
{
	/*concurrency::parallel_*/std::for_each(begin(Objs), end(Objs), [&](auto it) {
		it.second->ProcessCmd();
	});
}

void cGameScene::DispatchCmd(pCommand cmd)
{
	CurObj->SendCmd(cmd);
}

void cGameScene::Update()
{
	/*concurrency::parallel_*/std::for_each(begin(Objs), end(Objs), [](auto it) {
		it.second->Update();
	});
	this->Swap();
}

void cGameScene::Render(pCanvas & canvas)
{
	/*concurrency::parallel_*/std::for_each(begin(Objs), end(Objs), [&](auto it) {
		it.second->Render(canvas);
	});
}

void cGameScene::Swap()
{
	/*concurrency::parallel_*/std::for_each(begin(Objs), end(Objs), [](auto it) {
		it.second->Swap();
	});
}

cLocator& cGameScene::Locator()
{
	return *m_Locator;
}
