#include "StringUtility.h"

std::string trimLeft(const std::string &stringToTrim)
{
	std::string::size_type notSpacePosition = stringToTrim.find_first_not_of(' ');
	if(notSpacePosition < stringToTrim.size())
	{
		return stringToTrim.substr(notSpacePosition, stringToTrim.size()-notSpacePosition);
	}
	return stringToTrim;
}

std::string trimRight(const std::string &stringToTrim)
{
	std::string::size_type notSpacePosition = stringToTrim.find_last_not_of(' ');
	if(notSpacePosition < stringToTrim.size())
	{
		return stringToTrim.substr(0, notSpacePosition + 1);
	}
	return stringToTrim;
}

std::string trim(const std::string &stringToTrim)
{
	std::string resultString=trimLeft(stringToTrim);
	return trimRight(resultString);
}