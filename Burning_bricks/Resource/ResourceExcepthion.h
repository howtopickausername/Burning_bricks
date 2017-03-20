#pragma once
#include <stdexcept>

class cResourceExcep : public std::runtime_error
{
public:
	explicit cResourceExcep(const std::string& _Message)
		: std::runtime_error(_Message)
	{	// construct from message string
	}

	explicit cResourceExcep(const char *_Message)
		: std::runtime_error(_Message)
	{	// construct from message string
	}
};
