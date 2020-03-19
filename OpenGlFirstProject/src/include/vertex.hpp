#pragma once

#include <glm/glm.hpp>
#include <limits>

struct Vertex
{
	glm::vec3 v;
	glm::vec2 vt2D;
	glm::vec3 vt3D;
	glm::vec3 vn;
	unsigned int index;
	bool has3Dtexture = false;

	Vertex()
		: v{ std::numeric_limits<float>::max() }
		, vt2D{ std::numeric_limits<float>::max() }
		, vt3D{std::numeric_limits<float>::max()}
		, vn{ std::numeric_limits<float>::max() }
		, index{ std::numeric_limits<unsigned int>::max() }
	{
	}

	inline bool operator==(const Vertex& other)const
	{
		return (other.v == v && other.vt2D == vt2D && other.vt3D == vt3D && other.vn == vn);
	}
};