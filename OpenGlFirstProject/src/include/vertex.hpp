#pragma once

#include <glm/glm.hpp>
#include <limits>

struct Vertex
{
	glm::vec3 v;
	glm::vec2 vt;
	glm::vec3 vn;
	unsigned int index;

	Vertex()
		: v{ std::numeric_limits<float>::max() }
		, vt{ std::numeric_limits<float>::max() }
		, vn{ std::numeric_limits<float>::max() }
		, index{ std::numeric_limits<unsigned int>::max() }
	{
	}

	inline bool operator==(const Vertex& other)const
	{
		return (other.v == v && other.vt == vt && other.vn == vn);
	}
};