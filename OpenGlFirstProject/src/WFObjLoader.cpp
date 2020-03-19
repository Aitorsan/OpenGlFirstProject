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
	const std::string INDICES{"f"};
	const std::string EMTPY_STRING{" "};
	constexpr std::size_t TRIANGULAR_FACES{3};

}


WFObjLoader::WFObjLoader( const std::string& sourceFile)
	:FilePath{std::move(sourceFile)}
{
	CreateMesh();
}

void WFObjLoader::CreateMesh()
{
	std::vector<Positions> vPositions;
	std::vector<Normals> vNormals;
	std::vector<Textures> vTextures;
	std::vector<FaceIndices> fIndices;
	std::vector<Vertex> meshVertices;
	std::string data = ReadObjFile(FilePath);

	ParseObjData(data,vPositions,vTextures,vNormals,fIndices);

	if (vNormals.empty() && vTextures.empty())
	{
		// we have only positions
		for (auto& indices : fIndices)
		{
			Vertex v;
			Positions pos = vPositions[indices.front()];
			v.index = indices.front();
			v.v = { pos[0], pos[1],pos[2] };
			meshVertices.push_back(std::move(v));
		}

		for (auto& vertex : meshVertices)
		{
			Mesh.push_back(vertex.v.x);
			Mesh.push_back(vertex.v.y);
			Mesh.push_back(vertex.v.z);
		}
	}
	else if (vTextures.empty())// positions/Normals
	{
		// we have positions/normals
		for (auto& indices : fIndices)
		{
			Vertex v;
			Positions pos = vPositions[indices.front()];
			Normals norm = vNormals[indices.back()];
			v.index = indices.front();
			v.v = { pos[0], pos[1],pos[2] };
			v.vn = { norm[0], norm[1],norm[2] };

			meshVertices.push_back(std::move(v));
		}

		for (auto& vertex : meshVertices)
		{
			Mesh.push_back(vertex.v.x);
			Mesh.push_back(vertex.v.y);
			Mesh.push_back(vertex.v.z);
			Mesh.push_back(vertex.vn.x);
			Mesh.push_back(vertex.vn.y);
			Mesh.push_back(vertex.vn.z);

		}

	}
	else if (vNormals.empty())// positons/Textures
	{
		// we have positions/textures
		for (auto& indices : fIndices)
		{
			Vertex v;
			Positions pos = vPositions[indices.front()];
			Textures text = vTextures[indices.back()];
			v.index = indices.front();
			v.v = { pos[0], pos[1],pos[2] };
			if (text.size() < 3)
				v.vt2D = { text[0], text[1] };
			else
			{
				v.vt3D = { text[0], text[1],text[2] };
				v.has3Dtexture = true;
			}
			meshVertices.push_back(std::move(v));
		}

		for (auto& vertex : meshVertices)
		{
			Mesh.push_back(vertex.v.x);
			Mesh.push_back(vertex.v.y);
			Mesh.push_back(vertex.v.z);

			if (vertex.has3Dtexture)
			{
				Mesh.push_back(vertex.vt3D.x);
				Mesh.push_back(vertex.vt3D.y);
				Mesh.push_back(vertex.vt3D.z);
			}
			else
			{
				Mesh.push_back(vertex.vt2D.x);
				Mesh.push_back(vertex.vt2D.y);
			}

		}

	}
	else if( !vPositions.empty())// make sure nothing went wrong
	{
		//cool we have a full vertex the size is 3
		for (auto& indices : fIndices)
		{
			Vertex v;
			Positions pos =vPositions[indices.front()];
			Textures text = vTextures[indices.at(1)];
			Normals norm = vNormals[indices.back()];

			v.index = indices.front();
			v.v  = { pos[0], pos[1],pos[2] };
			v.vn = { norm[0], norm[1],norm[2] };

			if (text.size() < 3)
				v.vt2D = { text[0], text[1] };
			else
			{
				v.vt3D = { text[0], text[1],text[2] };
				v.has3Dtexture = true;
			}

			meshVertices.push_back(std::move(v));
		}

		for (auto& vertex : meshVertices)
		{
			Mesh.push_back(vertex.v.x);
			Mesh.push_back(vertex.v.y);
			Mesh.push_back(vertex.v.z);
			
			if (vertex.has3Dtexture)
			{
				Mesh.push_back(vertex.vt3D.x);
				Mesh.push_back(vertex.vt3D.y);
				Mesh.push_back(vertex.vt3D.z);
			}
			else
			{
				Mesh.push_back(vertex.vt2D.x);
				Mesh.push_back(vertex.vt2D.y);
			}
			
			Mesh.push_back(vertex.vn.x);
			Mesh.push_back(vertex.vn.y);
			Mesh.push_back(vertex.vn.z);
		}
	}
}

std::string WFObjLoader::ReadObjFile(const std::string& file)
{
	std::fstream f(file);

	if (!f.is_open())
		return std::string();
	
	std::string data{std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>()};

	return data;
}

void WFObjLoader::ParseObjData(const std::string& data, std::vector<Positions>& vP, std::vector<Textures>& vT, 
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



void WFObjLoader::AddVertexPositions(std::vector<Positions>& vps, std::vector<std::string>& vertexTokens)
{
	Positions vposition;
	for (std::string& token : vertexTokens)
	{
		float vp = str_to_number<float>(token);
		vposition.push_back(vp);
	}
	vps.push_back(std::move(vposition));
}

void WFObjLoader::AddTextureCoordinates(std::vector<Textures>& vts, std::vector<std::string>& textureTokens)
{
	Textures vtexture;
	for (std::string& token : textureTokens)
	{
		float vt = str_to_number<float>(token);
		vtexture.push_back(vt);
	}
	vts.push_back(std::move(vtexture));
}

void WFObjLoader::AddNormals(std::vector<Normals>& vn, std::vector<std::string>& normalsTokens)
{
	Normals vnormal;
	for (std::string& token : normalsTokens)
	{
		float vp = str_to_number<float>(token);
		vnormal.push_back(vp);
	}
	vn.push_back(std::move(vnormal));
}


void WFObjLoader::AddFaceIndices(std::vector<FaceIndices>& fi, std::vector<std::string>& facesTokens)
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

void WFObjLoader::TransformQuadFacesToTriangularFaces(std::vector<FaceIndices>& fi, const std::vector<std::string>& posIndices,
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



std::vector<unsigned int> WFObjLoader::GetIndices()
{
	return Indices;
}

std::vector<float>& WFObjLoader::GetMesh()
{
	return Mesh;
}



