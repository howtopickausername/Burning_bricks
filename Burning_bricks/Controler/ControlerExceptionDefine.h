#pragma once
#include <stdexcept>

class cControlerError : public std::runtime_error
{
public:
	explicit cControlerError(const std::string& _Message)
		: std::runtime_error(_Message.c_str())
	{	// construct from message string
	}

	explicit cControlerError(const char *_Message)
		: std::runtime_error(_Message)
	{	// construct from message string
	}
};