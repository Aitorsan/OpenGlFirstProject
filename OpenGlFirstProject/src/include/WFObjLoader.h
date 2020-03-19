#pragma once
#include "include/vertex.hpp"
#include <string>
#include <vector>

class WFObjLoader
{
public:
	using Positions = std::vector<float>;
	using Normals = std::vector<float>;
	using Textures = std::vector<float>;
	using FaceIndices = std::vector<int>;

	WFObjLoader(const std::string& filePath);	
	std::vector<unsigned int> GetIndices();
	std::vector<float>& GetMesh();
	
private:
	void CreateMesh();
	std::string ReadObjFile(const std::string& file);
	void ParseObjData(const std::string& d, std::vector<Positions>&, std::vector<Textures>&, std::vector<Normals>&, std::vector<FaceIndices>& );
	void AddNormals(std::vector<Normals>& vn, std::vector<std::string>& normalsTokens);
	void AddVertexPositions(std::vector<Positions>& vps, std::vector<std::string>& vertexTokens);
	void AddTextureCoordinates(std::vector<Textures>& vps, std::vector<std::string>& textureTokens);
	void AddFaceIndices(std::vector<FaceIndices>& fi, std::vector<std::string>& facesTokens);

	void TransformQuadFacesToTriangularFaces(std::vector<FaceIndices>& fi, const std::vector<std::string>& posIndices,
		const std::vector<std::string>& textureIndices,
		const std::vector<std::string>& normalIndices);

	const std::string& FilePath;	
	std::vector<unsigned int> Indices;
	std::vector<float> Mesh;
};

