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
	pGameObject monster = cMonsterFactory::CreateMonster();
	Objs.insert(std::make_pair(0, monster));
}

void cGameScene::DestroyScene()
{

}

void cGameScene::HandleInput()
{
	concurrency::parallel_for_each(begin(Objs), end(Objs), [&](auto it) {
		it.second->HandleInput();
	});
}

void cGameScene::HandleInput(cCommand const& cmd)
{
	CurObj->SendCmd(cmd);
}

void cGameScene::Update()
{
	concurrency::parallel_for_each(begin(Objs), end(Objs), [](auto it) {
		it.second->Update();
	});
}

cLocator& cGameScene::Locator()
{
	return *m_Locator;
}
