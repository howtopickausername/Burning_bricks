#pragma once
#include <sstream>
#include <ostream>

class cLog
{
public:
	cLog();
	cLog(std::ostream & out);
	~cLog();
	template<typename T>
	cLog& operator<<(T const& t);

private:
	std::ostream* logStream;
};

template<typename T>
cLog& cLog::operator<<(T const& t)
{
	if (logStream){
		std::stringstream ss;
		ss << t;
		(*logStream) << ss.str();
	}
	return *this;
}
