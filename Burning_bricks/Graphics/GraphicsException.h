#pragma once
#include <stdexcept>

class cGraphicsExcp : public std::runtime_error
{
public:
	explicit cGraphicsExcp(const std::string& _Message)
		: std::runtime_error(_Message)
	{	// construct from message string
	}

	explicit cGraphicsExcp(const char *_Message)
		: std::runtime_error(_Message)
	{	// construct from message string
	}
};