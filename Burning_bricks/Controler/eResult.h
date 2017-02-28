#pragma once
enum class eResult
{
	resAllGood = 0,		//
	resFalse = 1,		//function worked and returns 'false'
	resFailed = -1,		//function failed miserably
	resNotImpl = -2,	//function has not been implemented
	resForceDword =  0x7FFFFFFF
};
