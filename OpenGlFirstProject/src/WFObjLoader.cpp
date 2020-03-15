#include "include/WFObjLoader.h"
#include "include/utilities.hpp"
#include "include/Diagnostics.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

namespace
{
    const std::string VERTEX_POSITION{"v"};
	const std::string VERTEX_TEXTURE {"vt"};
	const std::string VERTEX_NORMAL  {"vn"};
	const std::string INDICES{ "f" };
	const std::string EMTPY_STRING{ " " };
}

WFObjLoader::WFObjLoader(const std::string& sourceFile)
	:FilePath{sourceFile}
{
	CreateShape();
}

void WFObjLoader::CreateShape()
{
	std::vector<glm::vec3> vPositions;
	std::vector<glm::vec2> vTextures;
	std::vector<glm::vec3> vNormals;
	std::vector<glm::uvec3> FIndices;

	ParseObjFile(vPositions,vTextures,vNormals,FIndices);

	std::vector<Vertex> raw_vertices;

	for (auto f_index : FIndices)
	{
		Vertex vert{};
		vert.v = vPositions[f_index.x];
		if (!vTextures.empty())
			vert.vt = vTextures[f_index.y];
		if (!vNormals.empty())
			vert.vn = vNormals[f_index.z];
		raw_vertices.push_back(std::move(vert));
	}

	std::vector<Vertex> NoDuplicateShapeVertices;
	unsigned int current_index = 0;
	// create indices for Index buffer object 
	for (auto& vert : raw_vertices)
	{
		auto begin_it = NoDuplicateShapeVertices.begin();
		auto end_it = NoDuplicateShapeVertices.end();
		auto result_it = std::find_if(begin_it, end_it, [&](Vertex& v)
		{
			return v == vert;
		});

		if (result_it != end_it)
		{
			Indices.push_back(result_it->index);
		}
		else
		{
			vert.index = current_index;
			Indices.push_back(current_index);
			++current_index;
			NoDuplicateShapeVertices.push_back(std::move(vert));
		}
	}

	for (auto& vertex : NoDuplicateShapeVertices)
	{
		RawShape.push_back(vertex.v.x);
		RawShape.push_back(vertex.v.y);
		RawShape.push_back(vertex.v.z);
		RawShape.push_back(vertex.vt.x);
		RawShape.push_back(vertex.vt.y);
		RawShape.push_back(vertex.vn.x);
		RawShape.push_back(vertex.vn.y);
		RawShape.push_back(vertex.vn.z);
	}
}

std::vector<unsigned int> WFObjLoader::GetIndices()
{
	return Indices;
}

std::vector<float> WFObjLoader::GetRawShape()
{
	return RawShape;
}

void WFObjLoader::ParseObjFile(std::vector<glm::vec3>& vPositions, std::vector<glm::vec2>& vTextures,
							   std::vector<glm::vec3>& vNormals, std::vector<glm::uvec3>& FIndices)
{
	std::ifstream file(FilePath,std::ifstream::in );
	if (!file.is_open())
	{
		LOG_ERROR( "opening file "+ FilePath );
		return;
	}
	//read the file into a string
	std::string data{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	file.close();

	std::vector<std::string>tokens;
	tokens = split_str(data, tokens,'\n');
	// fill in the vectors
	for (auto& token : tokens)
	{
		std::vector<std::string> splited_token;
		splited_token = split_str(token, splited_token);
		if (!splited_token.empty())
		{
			auto& vertex_type = splited_token.front();
			if (vertex_type == VERTEX_POSITION)
			{
				AddVertex(vPositions, splited_token);
			}
			else if (vertex_type == VERTEX_TEXTURE)
			{
				AddVertexTexture(vTextures, splited_token);
			}
			else if (vertex_type == VERTEX_NORMAL)
			{
				AddVertex(vNormals, splited_token);
			}
			else if (vertex_type == INDICES)
			{
				AddFIndex(FIndices, splited_token);
			}
		}
	}
}

std::vector<glm::vec3>& WFObjLoader::AddVertex(std::vector<glm::vec3>& vertices, std::vector<std::string>& splited_token)
{
	glm::vec3 values{};
	short position{ 0 };
	for (int i = 1; i < splited_token.size();++i)
	{
		std::stringstream buffer(splited_token[i]);
		float vertex;
		if (!(buffer >> vertex))
		{
			LOG_ERROR("failed converting vertex position into float: "+splited_token[i]);
		}
		else
		{
			values[position] = vertex;
			++position;
		}
	}
	vertices.push_back(values);
	return vertices;
}

std::vector<glm::vec2>& WFObjLoader::AddVertexTexture(std::vector<glm::vec2>& vTextures, std::vector<std::string>& splited_token)
{
	glm::vec2 values{};
	short position{ 0 };
	for (int i = 1; i < splited_token.size(); ++i)
	{
		std::stringstream buffer(splited_token[i]);
		float vertex;
		if (!(buffer >> vertex))
		{
			LOG_ERROR("failed converting vertex texture into float: " + splited_token[i]);
		}
		else
		{
			values[position] = vertex;
			++position;
		}
	}
	vTextures.push_back(values);
 	return vTextures;
}

std::vector<glm::uvec3>& WFObjLoader::AddFIndex(std::vector<glm::uvec3>& FIndices, std::vector<std::string>& splitedToken)
{  	// parse the indices but subtracting one from them
	for (int i = 1; i < splitedToken.size(); ++i)
	{
		std::vector<std::string> ts;
		glm::uvec3 findice;
		auto& currToken = splitedToken[i];
		ts = split_str(currToken, ts,'/');
		if (ts.empty())
		{
			// case where no / are found means we only have vertex positions
			ts = split_str(currToken, ts);
			findice[0] = str_to_number<unsigned int>(ts.front()) - 1;
			findice[1] = str_to_number<unsigned int>(ts.at(1)) - 1;
			findice[2] = str_to_number<unsigned int>(ts.back()) - 1;

		}
		// If we find / and the size is 2 it means 
		// we have position and texture coordinates but not normals
		else if( ts.size() == 2)
		{   
			findice[0] = str_to_number<unsigned int>(ts.front()) - 1;
			findice[1] = str_to_number<unsigned int>(ts.back()) - 1;
		}
		// If we find // the size will be 3 because we add one empty extra string in place of one of the '/' 
	    // This means we have position and normals but not texture coordinates
		else if (ts.size() == 3)
		{
			findice[0] = str_to_number<unsigned int>(ts.front()) - 1;
			if (ts.at(1) != EMTPY_STRING)
			{
				findice[1] = str_to_number<unsigned int>(ts.at(1)) - 1;
			}
			findice[2] = str_to_number<unsigned int>(ts.back()) - 1;
		}
		FIndices.push_back(findice);
	}	
	return FIndices;
}



