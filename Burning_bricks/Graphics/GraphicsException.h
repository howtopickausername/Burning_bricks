#pragma once
#include <stdexcept>
#include "GraphicsDefine.h"

_GP_BEGIN;
class Exception : public std::runtime_error
{
public:
	explicit Exception(const std::string& _Message)
		: std::runtime_error(_Message)
	{	// construct from message string
	}

	explicit Exception(const char *_Message)
		: std::runtime_error(_Message)
	{	// construct from message string
	}
};
_GP_END;