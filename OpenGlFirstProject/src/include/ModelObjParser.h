/**************************************************************************************************************************************
  This is an simple obj loader for 3D models. It just parses vertex positions, normals , and 3D or 2D texture coordinates.
  
  There are two ways of getting data from the loader:

  1. Just called the GetIndices and GetModelData functions wich will return a vector of floats contaning all the vertex data
     notice you will have to get the vertex sizes as well.

  2. Define your on struct and call the fillVertexSizes,fillModelData, and fillIndexData to fill in the vertex data.
     The struct or class passed in must contain the following public members:
      
	  - std::vector<float> for the vertex data
	  - std::vector<unsigned int> for the index data;
	  - three int variables for the vertex/texture/normals size, if one of this vertex is missing it will be filled with a 0 value.
  
  ¡WARNING! : THE LOADER CANNOT LOAD:

  1. Compose models that do not have the same number of vertices
    
	 e.g: 1//2 1//3 2//3  and 1/2/3 2/3/5 3/4/8
	 
	 As we can see one of the models in the file has only normals and positions while the other has positions/texture/normals
	 this will crash the parser we can have one of the options only.( Maybe can be fixed in the future) 

  2. Negative indexing formtas : -1/-2/-3 are not supported ( no intention to support it) 

  3. Does not load materials ( Maybe can be added in the future) 

  3. Does not load vp vertices and l attributes ( no intention to add it) 

  Other issue of interest is that a complex model can take up to 2 minutes loading

**********************************************************************************************************************************/
#pragma once

#include "include/vertex.hpp"
#include <string>
#include <vector>

class ModelObjParser
{
	using Positions = std::vector<float>;
	using Normals = std::vector<float>;
	using Textures = std::vector<float>;
	using FaceIndices = std::vector<int>;

	std::vector<unsigned int> Indices;
	std::vector<float> ModelRawData;
	std::vector<float> IndexedModelData;
	int NormVertexLength;
	int TextureVertexLength;
	
public:
	enum GENOPTION
	{
		GEN_INDICES = 2, NO_GEN_INDICES = 1
	};

	ModelObjParser();	
	void ParseModelData(const std::string& modelData, GENOPTION option);
	std::vector<unsigned int> GetIndices();
	std::vector<float>& GetModelDataForIndexing();
	std::vector<float>& GetModelDataWithNoIndices();
	void fillVertexSizes(int& vertexPositionSize, int& vertexTextureSize, int& vertexNormalSize);
	

	template <class Mesh> inline void fillModelData(Mesh& shape, std::vector<float> Mesh::* memPtr)
	{
		shape.*memPtr = IndexedModelData;
	}
	
	template <class Mesh> inline void fillIndexData(Mesh& shape, std::vector<unsigned int> Mesh::* memPtr)
	{
		shape.*memPtr = Indices;
	}

private:
	void ParseObjData(const std::string& d, std::vector<Positions>&, std::vector<Textures>&, std::vector<Normals>&, std::vector<FaceIndices>& );
	void AddNormals(std::vector<Normals>& vn, std::vector<std::string>& normalsTokens);
	void AddVertexPositions(std::vector<Positions>& vps, std::vector<std::string>& vertexTokens);
	void AddTextureCoordinates(std::vector<Textures>& vps, std::vector<std::string>& textureTokens);
	void AddFaceIndices(std::vector<FaceIndices>& fi, std::vector<std::string>& facesTokens);
	void TransformQuadFacesToTriangularFaces(std::vector<FaceIndices>& fi, const std::vector<std::string>& posIndices,
											const std::vector<std::string>& textureIndices,const std::vector<std::string>& normalIndices);

};

