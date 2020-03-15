#pragma once
#include "include/vertex.hpp"
#include <string>
#include <vector>

class WFObjLoader
{
public:
	WFObjLoader(const std::string& filePath);	
	std::vector<unsigned int> GetIndices();
	std::vector<float> GetRawShape();
private:
	void CreateShape();
	void ParseObjFile(std::vector<glm::vec3>& vPositions,
	                  std::vector<glm::vec2>& vTextures,
	                  std::vector<glm::vec3>& vNormals,
	                  std::vector<glm::uvec3>& FIndices );
	std::vector<glm::vec3>& AddVertex(std::vector<glm::vec3>& vertices, std::vector<std::string>& splited_token);
	std::vector<glm::vec2>& AddVertexTexture(std::vector<glm::vec2>& vTextures,std::vector<std::string>& splited_token);
	std::vector<glm::uvec3>& AddFIndex(std::vector<glm::uvec3>& FIndices,std::vector<std::string>& splited_token);


	const std::string& FilePath;	
	std::vector<unsigned int> Indices;
	std::vector<float> RawShape;
};

