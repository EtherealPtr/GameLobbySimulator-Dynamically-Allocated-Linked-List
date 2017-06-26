#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#include <strstream>
#include <sstream>
#include <string>

template<typename T>
std::string ToString(const T& _value)
{
	std::strstream theStream;
	theStream << _value << std::ends;
	return (theStream.str());
}

template<typename T>
std::wstring ToWideString(const T& _value)
{
	std::wstringstream theStream;
	theStream << _value << std::ends;
	return (theStream.str());
}

#endif // !__UTILS_H__