#pragma once
#include <memory>
#include <map>
#include "StateBreed.h"

class cStateMap
{
public:
	typedef std::map<int, cStateBreed> _MType;
	cStateMap();
	cStateMap(std::shared_ptr<_MType> parent);
	~cStateMap();

	std::shared_ptr<_MType> ParentMap;
	_MType Map;
};
typedef std::shared_ptr<cStateMap> pStateMap;