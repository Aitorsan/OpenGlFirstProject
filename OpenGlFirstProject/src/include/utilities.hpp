#pragma once
#include <string>

// Returns an empty container , if the delimiter separator was not found
template <class Container>
inline Container& split_str(const std::string& str, Container& cont,char delim = ' ')
{
	std::size_t current, previous = 0;
	current = str.find(delim);
	while (current != std::string::npos)
	{
		cont.push_back(str.substr(previous, current - previous));
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