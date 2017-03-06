#include "stdafx.h"
#include "Log.h"

cLog::cLog()
	:logStream(nullptr)
{

}

cLog::cLog(std::ostream & out)
	:logStream(&out)
{

}

cLog::~cLog()
{

}
