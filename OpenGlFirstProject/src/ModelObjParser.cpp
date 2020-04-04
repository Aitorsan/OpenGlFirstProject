#include "include/ModelObjParser.h"
#include "include/utilities.hpp"
#include "include/Diagnostics.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>

namespace
{
    const std::string VERTEX_POSITION{"v"};
	const std::string VERTEX_TEXTURE {"vt"};
	const std::string VERTEX_NORMAL  {"vn"};
	const std::string INDICES{"f"};
	const std::string EMTPY_STRING{" "};
	constexpr std::size_t TRIANGULAR_FACES{3};
}

ModelObjParser::ModelObjParser( )
	: Indices{}
	, ModelRawData{}
	, IndexedModelData{}
	, NormVertexLength{ 0}
	, TextureVertexLength{0}
{
}

void ModelObjParser::ParseModelData(const std::string& modelData, GENOPTION option)
{
	std::vector<Positions> vPositions;
	std::vector<Normals> vNormals;
	std::vector<Textures> vTextures;
	std::vector<FaceIndices> fIndices;
	std::vector<Vertex> ModelRawDataVertices;
	
	ParseObjData(modelData,vPositions,vTextures,vNormals,fIndices);

	if (vNormals.empty() && vTextures.empty())
	{
		// we have only positions
		for (auto& indices : fIndices)
		{
			Vertex v;
			const int& posIndex = indices.front();
			Positions pos = vPositions[posIndex];
			v.index = posIndex;
			v.v = { pos[0], pos[1],pos[2] };
			ModelRawDataVertices.push_back(std::move(v));
		}
	}
	else if (vTextures.empty())// we have positions/Normals
	{
		NormVertexLength = 3;
		
		for (auto& indices : fIndices)
		{
			Vertex v;
			const int& posIndex = indices.front();
			const int& normIndex = indices.at(1);
			Positions pos = vPositions[posIndex];
			Normals norm = vNormals[normIndex];
			v.index = posIndex;
			v.vnIndex = normIndex;
			v.v = { pos[0], pos[1],pos[2] };
			v.vn = { norm[0], norm[1],norm[2] };

			ModelRawDataVertices.push_back(std::move(v));
		}
	}
	else if (vNormals.empty())// we have  positons/Textures
	{
		TextureVertexLength = 2;

		for (auto& indices : fIndices)
		{
			Vertex v;
			const int& posIndex = indices.front();
			const int& textIndex = indices.at(1);
			Positions pos = vPositions[posIndex];
			Textures text = vTextures[textIndex];
			v.index = posIndex;
			v.vtIndex = textIndex;
			v.v = { pos[0], pos[1],pos[2] };
			if (text.size() < 3)
				v.vt2D = { text[0], text[1] };
			else
			{
				v.vt3D = { text[0], text[1],text[2] };
				v.has3Dtexture = true;
			}
			ModelRawDataVertices.push_back(std::move(v));
		}

		Vertex& lastVertex = ModelRawDataVertices.back();

		if (lastVertex.has3Dtexture)
				TextureVertexLength = 3;

	}
	else if( !vPositions.empty())// make sure nothing went wrong
	{
		//cool we have a full vertex the size is 3
		NormVertexLength = 3;

		for (auto& indices : fIndices)
		{
			Vertex v;
			const int& posIndex = indices.front();
			const int& textIndex = indices.at(1);
			const int& normIndex = indices.at(2);
			Normals norm = vNormals[normIndex];
			Positions pos =vPositions[posIndex];
			Textures text = vTextures[textIndex];

			v.index = posIndex;
			v.vtIndex = textIndex;
			v.vnIndex = normIndex;
			v.v  = { pos[0], pos[1],pos[2] };
			v.vn = { norm[0], norm[1],norm[2] };

			if (text.size() < 3)
				v.vt2D = { text[0], text[1] };
			else
			{
				v.vt3D = { text[0], text[1],text[2] };
				v.has3Dtexture = true;
			}
			
			ModelRawDataVertices.push_back(std::move(v));
		}

		Vertex& lastVertex = ModelRawDataVertices.back();

		if (lastVertex.has3Dtexture)
			TextureVertexLength = 3;
		else
			TextureVertexLength = 2;
	}

	//create crresponding indexBuffers and remove duplicates 
	std::map<Vertex, int> indexMap;
    std::vector<Vertex> noRepeatedVerticesData;
	
	int currentIndex = 0;
	
	for (int i = 0; i < ModelRawDataVertices.size(); ++i)
	{
		Vertex& vt = ModelRawDataVertices[i];
		std::pair<std::map<Vertex, int>::iterator, bool> value = indexMap.insert(std::pair<Vertex, int>{vt, 0});
		// value.second == false if an equivalent key already existed
		if (value.second || i == 0)
		{
			indexMap[vt] = currentIndex;
			Indices.push_back(currentIndex);
			noRepeatedVerticesData.push_back(vt);
			++currentIndex;
		}
		else if (vt == (value.first)->first)// the value already exist
		{
			Indices.push_back((value.first)->second);
		}
		else// at this point the keys are seen as equal by the map because they have same index
		{
			// To avoid the map to remove the "similar" vertices , we change the index so we can have duplicate keys
			// because we really don´t care about the values of the index in the map at this point we just use the map
			// as a look up so it is quicker than iterating through a vector 
			vt.index = -1;
			indexMap[(value.first)->first] = (value.first)->second;
			indexMap[vt] = currentIndex;
			Indices.push_back(currentIndex);
			noRepeatedVerticesData.push_back(vt);
			++currentIndex;
		}
	}


	if (option & NO_GEN_INDICES)
	{
		// Vertex data with no indices to use with glDrawArrays()
		for (auto& vertex : ModelRawDataVertices)
		{
			ModelRawData.push_back(vertex.v.x);
			ModelRawData.push_back(vertex.v.y);
			ModelRawData.push_back(vertex.v.z);

			if (TextureVertexLength != 0)
			{
				if (vertex.has3Dtexture)
				{
					ModelRawData.push_back(vertex.vt3D.x);
					ModelRawData.push_back(vertex.vt3D.y);
					ModelRawData.push_back(vertex.vt3D.z);
				}
				else
				{
					ModelRawData.push_back(vertex.vt2D.x);
					ModelRawData.push_back(vertex.vt2D.y);
				}
			}
			if (NormVertexLength != 0)
			{
				ModelRawData.push_back(vertex.vn.x);
				ModelRawData.push_back(vertex.vn.y);
				ModelRawData.push_back(vertex.vn.z);
			}
		}
	}
	
	if (option & GEN_INDICES)
	{
		// Vertex data with  indices to use with glDrawElements()
		for (auto& vertex : noRepeatedVerticesData)
		{
			IndexedModelData.push_back(vertex.v.x);
			IndexedModelData.push_back(vertex.v.y);
			IndexedModelData.push_back(vertex.v.z);

			if (TextureVertexLength != 0)
			{
				if (vertex.has3Dtexture)
				{
					IndexedModelData.push_back(vertex.vt3D.x);
					IndexedModelData.push_back(vertex.vt3D.y);
					IndexedModelData.push_back(vertex.vt3D.z);
				}
				else
				{
					IndexedModelData.push_back(vertex.vt2D.x);
					IndexedModelData.push_back(vertex.vt2D.y);
				}
			}
			if (NormVertexLength != 0)
			{
				IndexedModelData.push_back(vertex.vn.x);
				IndexedModelData.push_back(vertex.vn.y);
				IndexedModelData.push_back(vertex.vn.z);
			}
		}
	}

}

void ModelObjParser::ParseObjData(const std::string& data, std::vector<Positions>& vP, std::vector<Textures>& vT, 
	                                            std::vector<Normals>& vN, std::vector<FaceIndices>& fI)
{
	std::vector<std::string> tokens;
	split_str(data, tokens, '\n');
	// fill in the vectors
	for (std::string& token : tokens)
	{
		std::vector<std::string> splitedToken;
		split_str(token, splitedToken);

		if (!splitedToken.empty())
		{
			auto& vertex_type = splitedToken.front();
			if (vertex_type == VERTEX_POSITION)
			{ 
				splitedToken.erase(splitedToken.begin());
				AddVertexPositions(vP, splitedToken);
			}
			else if (vertex_type == VERTEX_TEXTURE)
			{
				splitedToken.erase(splitedToken.begin());
				AddTextureCoordinates(vT, splitedToken);
			}
			else if (vertex_type == VERTEX_NORMAL)
			{
				splitedToken.erase(splitedToken.begin());
				AddNormals(vN, splitedToken);
			}
			else if (vertex_type == INDICES)
			{
				splitedToken.erase(splitedToken.begin());
				AddFaceIndices(fI, splitedToken);
			}
		}
	}
}

void ModelObjParser::AddVertexPositions(std::vector<Positions>& vps, std::vector<std::string>& vertexTokens)
{
	Positions vposition;
	for (std::string& token : vertexTokens)
	{
		float vp = str_to_number<float>(token);
		vposition.push_back(vp);
	}
	vps.push_back(std::move(vposition));
}

void ModelObjParser::AddTextureCoordinates(std::vector<Textures>& vts, std::vector<std::string>& textureTokens)
{
	Textures vtexture;
	for (std::string& token : textureTokens)
	{
		float vt = str_to_number<float>(token);
		vtexture.push_back(vt);
	}
	vts.push_back(std::move(vtexture));
}

void ModelObjParser::AddNormals(std::vector<Normals>& vn, std::vector<std::string>& normalsTokens)
{
	Normals vnormal;
	for (std::string& token : normalsTokens)
	{
		float vp = str_to_number<float>(token);
		vnormal.push_back(vp);
	}
	vn.push_back(std::move(vnormal));
}


void ModelObjParser::AddFaceIndices(std::vector<FaceIndices>& fi, std::vector<std::string>& facesTokens)
{
		std::vector<std::string> posIndices;
		std::vector<std::string> textureIndices;
		std::vector<std::string> normalIndices;
		for (int i = 0; i < facesTokens.size(); ++i)
		{
			std::string& token = facesTokens[i];

			std::vector<std::string> faceIndices;
			faceIndices = split_str(token, faceIndices, '/');
			std::size_t tSize = faceIndices.size();

			if (tSize == 0 && !token.empty())
			{  
				posIndices.push_back(token);
			}
			else if (tSize == 3)
			{
				posIndices.push_back(faceIndices.front());
				textureIndices.push_back(faceIndices.at(1));
				normalIndices.push_back(faceIndices.back());
			}
			else if (tSize == 2)
			{
				// count the number of / to understand if we have positions/ textures or positions/ nomals
				size_t n = std::count(token.begin(), token.end(), '/');
				if (n == 1) // positions/ textures
				{
					posIndices.push_back(faceIndices.front());
					textureIndices.push_back(faceIndices.back());
				}
				else if (n == 2)// positions/normals
				{
					posIndices.push_back(faceIndices.front());
					normalIndices.push_back(faceIndices.back());
				}
			}
		}

		//if the format is specified in quads we need to create triangular faces oursevles
		if (facesTokens.size() > TRIANGULAR_FACES)
		{
			TransformQuadFacesToTriangularFaces(fi, posIndices,textureIndices,normalIndices);
		}
		else
		{
			for (int i = 0; i < posIndices.size(); ++i)
			{
				FaceIndices faces{};
				faces.push_back(str_to_number<unsigned int>(posIndices[i]) - 1);
				if (!textureIndices.empty())
					faces.push_back(str_to_number<unsigned int>(textureIndices[i]) - 1);
				if (!normalIndices.empty())
					faces.push_back(str_to_number<unsigned int>(normalIndices[i]) - 1);
				fi.push_back(std::move(faces));
			}
		}
}

void ModelObjParser::TransformQuadFacesToTriangularFaces(std::vector<FaceIndices>& fi, const std::vector<std::string>& posIndices,
																				const std::vector<std::string>& textureIndices,
																				const std::vector<std::string>& normalIndices)
{
	// transform in tirangular faces
	// eg: 1 2 3 4  -> 1 2 3 1 3 4
	std::vector<std::string> triangularIndexPositions{};
	std::size_t np = posIndices.size();
	for (int i = 0; i < np - 2; ++i)
	{
		triangularIndexPositions.push_back(posIndices[0]);
		triangularIndexPositions.push_back(posIndices[i + 1]);
		triangularIndexPositions.push_back(posIndices[i + 2]);
	}

	std::vector<std::string> triangularIndexTextures{};

	if (!textureIndices.empty())
	{
		std::size_t nt = textureIndices.size();
		for (int i = 0; i < nt - 2; ++i)
		{
			triangularIndexTextures.push_back(textureIndices[0]);
			triangularIndexTextures.push_back(textureIndices[i + 1]);
			triangularIndexTextures.push_back(textureIndices[i + 2]);
		}
	}

	std::vector<std::string> triangularIndexNormals{};
	if (!normalIndices.empty())
	{
		std::size_t n = normalIndices.size();
		for (int i = 0; i < n - 2; ++i)
		{
			triangularIndexNormals.push_back(normalIndices[0]);
			triangularIndexNormals.push_back(normalIndices[i + 1]);
			triangularIndexNormals.push_back(normalIndices[i + 2]);
		}
	}
	
	for (int i = 0; i < triangularIndexPositions.size(); ++i)
	{
		FaceIndices faces{};
		faces.push_back(str_to_number<unsigned int>(triangularIndexPositions[i]) - 1);
		if (!triangularIndexTextures.empty() && i < triangularIndexTextures.size())
			faces.push_back(str_to_number<unsigned int>(triangularIndexTextures[i]) - 1);
		if (!triangularIndexNormals.empty() && i < triangularIndexNormals.size())
			faces.push_back(str_to_number<unsigned int>(triangularIndexNormals[i]) - 1);
		fi.push_back(std::move(faces));
	}
}

std::vector<unsigned int> ModelObjParser::GetIndices()
{
	return Indices;
}

std::vector<float>& ModelObjParser::GetModelDataWithNoIndices()
{
	return ModelRawData;
}

std::vector<float>& ModelObjParser::GetModelDataForIndexing()
{
	return IndexedModelData;
}

void ModelObjParser::fillVertexSizes(int& vertexPositionSize, int& vertexTextureSize, int& vertexNormalSize)
{
	vertexPositionSize = 3;
	vertexTextureSize = TextureVertexLength;
	vertexNormalSize = NormVertexLength;
}

