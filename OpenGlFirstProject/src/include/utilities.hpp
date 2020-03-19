#pragma once
#include <string>

/******************************************************************************
   Splits string into tokens with a given delimiter, and add them into a vector.
   NOTE: if a token is an empty string, meaning an extra space it will be romoved
   and will not be added to the list of tokens.
   Returns an empty container , if the delimiter separator was not found.
********************************************************************************/
template <class Container>
inline Container& split_str(const std::string& str, Container& cont,char delim = ' ')
{
	std::size_t current, previous = 0;
	current = str.find(delim);
	while (current != std::string::npos)
	{
		std::string s = str.substr(previous, current - previous);
		if (!s.empty())
		{
			s.erase(std::remove(s.begin(), s.end(), '\t'), s.end());
			s.erase(std::remove(s.end()-1, s.end(), ' '), s.end());
			cont.push_back(std::move(s));
		}
		previous = current + 1;
		current = str.find(delim, previous);
	}
	if( !cont.empty())
		cont.push_back(str.substr(previous, current - previous));
	return cont;
}

template <typename T>
T str_to_number(std::string& str)
{
	T number;
	std::stringstream ss(str);
	ss >> number;
	if (ss.fail())
	{
		std::runtime_error e(str);
		throw e;
	}
	return number;
}