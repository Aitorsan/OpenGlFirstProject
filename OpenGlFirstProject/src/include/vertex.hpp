#pragma once
#include <glm/glm.hpp>
#include <limits>
#include <iostream>

struct Vertex
{
	glm::vec3 v;
	glm::vec2 vt2D;
	glm::vec3 vt3D;
	glm::vec3 vn;
	unsigned int index;
	unsigned int vtIndex;
	unsigned int vnIndex;
	bool has3Dtexture = false;
	
	Vertex()
		: v{ std::numeric_limits<float>::max() }
		, vt2D{ std::numeric_limits<float>::max() }
		, vt3D{std::numeric_limits<float>::max()}
		, vn{ std::numeric_limits<float>::max() }
		, index{ std::numeric_limits<unsigned int>::max() }
	    , vtIndex{ std::numeric_limits<unsigned int>::max() }
	    , vnIndex{ std::numeric_limits<unsigned int>::max() }
	{
	}

	inline bool operator==(const Vertex& other) const
	{
		return  (other.index == index && other.vtIndex == vtIndex && other.vnIndex == vtIndex && other.vnIndex == vnIndex) && (other.has3Dtexture == has3Dtexture);
	}
	inline bool operator<(const Vertex& other)  const
	{       
		return index < other.index;
	}
};