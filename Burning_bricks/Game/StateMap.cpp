#include "stdafx.h"
#include "StateMap.h"


cStateMap::cStateMap()
{

}

cStateMap::cStateMap(std::shared_ptr<_MType> parent)
	:ParentMap(parent)
{

}

cStateMap::~cStateMap()
{

}

void cStateMap::Insert(cStateBreed const & sb) {
	Map.insert(std::make_pair(sb.Id, sb));
}

cStateBreed const & cStateMap::At(cStateBreed::IdType id) {
	_MType::iterator t = Map.find(id);
	if (t == Map.end()) {
		t = ParentMap->find(id);
		if (t == ParentMap->end()) {
			throw std::invalid_argument("cStateMap::operator[]-Could not find" + std::to_string(id));
		}
	}
	return t->second;
}
