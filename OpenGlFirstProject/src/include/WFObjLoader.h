#pragma once
#include "include/vertex.hpp"
#include <string>
#include <vector>


class WFObjLoader
{
	using Positions = std::vector<float>;
	using Normals = std::vector<float>;
	using Textures = std::vector<float>;
	using FaceIndices = std::vector<int>;

public:
	enum GENOPTION
	{
		GEN_INDICES = 0, NO_GEN_INDICES = 1
	};
	
	WFObjLoader(const std::string& modelData, GENOPTION option);	
	std::vector<unsigned int> GetIndices();
	std::vector<float>& GetModelDataForIndexing();
	std::vector<float>& GetModelDataWithNoIndices();
	
private:
	void CreateModelRawData(const std::string& modelData);
	void ParseObjData(const std::string& d, std::vector<Positions>&, std::vector<Textures>&, std::vector<Normals>&, std::vector<FaceIndices>& );
	void AddNormals(std::vector<Normals>& vn, std::vector<std::string>& normalsTokens);
	void AddVertexPositions(std::vector<Positions>& vps, std::vector<std::string>& vertexTokens);
	void AddTextureCoordinates(std::vector<Textures>& vps, std::vector<std::string>& textureTokens);
	void AddFaceIndices(std::vector<FaceIndices>& fi, std::vector<std::string>& facesTokens);
	void TransformQuadFacesToTriangularFaces(std::vector<FaceIndices>& fi, const std::vector<std::string>& posIndices,
											const std::vector<std::string>& textureIndices,const std::vector<std::string>& normalIndices);
	
	std::vector<unsigned int> Indices;
	std::vector<float> ModelRawData;
	std::vector<float> IndexedModelData;
	int posVertexLength = 3;
	int normVertexLength = -1;
	int textureVertexLength = -1;
};

