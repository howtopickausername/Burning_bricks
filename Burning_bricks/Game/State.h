#pragma once
#include <stack>
#include "StateMap.h"

class cGameObject;
class cState
{
public:
	cState(pStateMap map);
	~cState();
	void Init(cGameObject& obj);
	void Update(cGameObject& obj);
	void HandleInput(cGameObject& obj, pCommand cmd);
	const pStateBreed GetBreed() const;
	std::shared_ptr<cState> Copy() const;

private:
	bool CheckNewBreed(cGameObject& obj);

	pStateMap StateMap;
	pStateBreed StateBreed;
	std::stack<cStateBreed::IdType> StateStack;
	pSbRtn NewBreed;	//�����л�����һ��״̬״̬
};
typedef std::shared_ptr<cState> pState;