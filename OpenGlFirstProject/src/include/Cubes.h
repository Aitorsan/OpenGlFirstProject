

void genSkyBoxCube(float (&skybox)[108])
{
	float skyBoxVertices[]
	{
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	std::copy(skyBoxVertices, skyBoxVertices + 108, skybox);
}
template <int VertexSize,int IndicesSize>
void genCubeWithTextureCoordsAndIndices(float (&cubeVert)[VertexSize], unsigned int(&indices)[IndicesSize])
{
	constexpr int vertSize = 192;
    constexpr int indSize = 36;
	
	//cubes
	constexpr float vertices[vertSize]
	{   //back face
	   -0.5f, -0.5f, -0.5f,	   0.0f, 0.0f, -1.0f,		0.0f, 0.0f,  //0
		0.5f, -0.5f, -0.5f,	   0.0f, 0.0f, -1.0f,		1.0f, 0.0f,  //1
		0.5f,  0.5f, -0.5f,	   0.0f, 0.0f, -1.0f,		1.0f, 1.0f,  //2
	   -0.5f,  0.5f, -0.5f,	   0.0f, 0.0f, -1.0f,	    0.0f, 1.0f,  //3//0
		//front face
	   -0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    0.0f, 0.0f,  //4
		0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    1.0f, 0.0f,  //5
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    1.0f, 1.0f,  //6  //6
	   -0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    0.0f, 1.0f,  //7//4
	   //left face
	   -0.5f,  0.5f,  0.5f,	   -1.0f,  0.0f,  0.0f,		1.0f, 0.0f,  //8
	   -0.5f,  0.5f, -0.5f,	   -1.0f,  0.0f,  0.0f,		1.0f, 1.0f,  //9
	   -0.5f, -0.5f, -0.5f,	   -1.0f,  0.0f,  0.0f,		0.0f, 1.0f,  //10 //10
	   -0.5f, -0.5f,  0.5f,	   -1.0f,  0.0f,  0.0f,		0.0f, 0.0f,  //11//8
		//right face
		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f, //12
		0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 1.0f, //13
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f, //14//14
		0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 0.0f, //15//12
		//bottom face
	   -0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 1.0f, //16
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f, //17
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f, //18//18
	   -0.5f, -0.5f,  0.5f,	    0.0f, -1.0f,  0.0f,		0.0f, 0.0f, //19//16
	   //top face
	   -0.5f,  0.5f, -0.5f,	    0.0f,  1.0f,  0.0f,		0.0f, 1.0f, //20
		0.5f,  0.5f, -0.5f,	    0.0f,  1.0f,  0.0f,		1.0f, 1.0f, //21
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f, //22//22
	   -0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f, //23//20
	};

	// cube indices 
	constexpr unsigned int cube_indices[IndicesSize]
	{
		0, 1, 2, 2, 3, 0,    // back face
		4, 5, 6, 6, 7, 4,    // front face
		8, 9, 10, 10, 11, 8, // left face
		12,13,14,14,15,12,   // right face
		16,17,18,18,19,16,   // bottom face
		20,21,22,22,23,20    // up face
	};
	static_assert(vertSize == VertexSize);
	static_assert(indSize == IndicesSize);
	std::copy(vertices, vertices + vertSize, cubeVert);
	std::copy(cube_indices, cube_indices + indSize, indices);
}

void getCube(float(&cube)[36*6])
{
	float internalCube[36*6] =
	{
		//back face - orange
	   -0.5f, -0.5f, -0.5f,     0.9f, 0.5f,0.0f,
		0.5f, -0.5f, -0.5f,     0.9f, 0.5f,0.0f,
		0.5f,  0.5f, -0.5f,     0.9f, 0.5f,0.0f,
		0.5f,  0.5f, -0.5f,     0.9f, 0.5f,0.0f,
	   -0.5f,  0.5f, -0.5f,     0.9f, 0.5f,0.0f,
	   -0.5f, -0.5f, -0.5f,     0.9f, 0.5f,0.0f,
	   //front face -- green
	   -0.5f, -0.5f,  0.5f,     0.0f, 1.0f,0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, 1.0f,0.0f,
		0.5f,  0.5f,  0.5f,     0.0f, 1.0f,0.0f,
		0.5f,  0.5f,  0.5f,     0.0f, 1.0f,0.0f,
	   -0.5f,  0.5f,  0.5f,     0.0f, 1.0f,0.0f,
	   -0.5f, -0.5f,  0.5f,     0.0f, 1.0f,0.0f,
	   //left face--- blue
	   -0.5f,  0.5f,  0.5f,     0.0f, 0.0f,1.0f,
	   -0.5f,  0.5f, -0.5f,     0.0f, 0.0f,1.0f,
	   -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,1.0f,
	   -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,1.0f,
	   -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,1.0f,
	   -0.5f,  0.5f,  0.5f,     0.0f, 0.0f,1.0f,
	   //right face
		0.5f,  0.5f,  0.5f,     1.0f, 0.0f,1.0f,
		0.5f,  0.5f, -0.5f,     1.0f, 0.0f,1.0f,
		0.5f, -0.5f, -0.5f,     1.0f, 0.0f,1.0f,
		0.5f, -0.5f, -0.5f,     1.0f, 0.0f,1.0f,
		0.5f, -0.5f,  0.5f,     1.0f, 0.0f,1.0f,
		0.5f,  0.5f,  0.5f,     1.0f, 0.0f,1.0f,
		//buttom  
	   -0.5f, -0.5f, -0.5f,     0.5f, 0.5f,1.0f,
		0.5f, -0.5f, -0.5f,     0.5f, 0.5f,1.0f,
		0.5f, -0.5f,  0.5f,     0.5f, 0.5f,1.0f,
		0.5f, -0.5f,  0.5f,     0.5f, 0.5f,1.0f,
	   -0.5f, -0.5f,  0.5f,     0.5f, 0.5f,1.0f,
	   -0.5f, -0.5f, -0.5f,     1.0f, 1.0f,1.0f,
	   //top                    red
	   -0.5f,  0.5f, -0.5f,     1.0f, 0.0f,0.0f,
		0.5f,  0.5f, -0.5f,     1.0f, 0.0f,0.0f,
		0.5f,  0.5f,  0.5f,     1.0f, 0.0f,0.0f,
		0.5f,  0.5f,  0.5f,     1.0f, 0.0f,0.0f,
	   -0.5f,  0.5f,  0.5f,     1.0f, 0.0f,0.0f,
	   -0.5f,  0.5f, -0.5f,     1.0f, 0.0f,0.0f
	};

	std::copy(std::begin(internalCube), std::end(internalCube), std::begin(cube));
}

void getWhiteCube(float(&cube)[36 * 6])
{
	float internalCube[36 * 6] =
	{
		//back face - orange
	   -0.5f, -0.5f, -0.5f,     1.f, 1.f,1.f,
		0.5f, -0.5f, -0.5f,     1.f, 1.f,1.f,
		0.5f,  0.5f, -0.5f,     1.f, 1.f,1.f,
		0.5f,  0.5f, -0.5f,     1.f, 1.f,1.f,
	   -0.5f,  0.5f, -0.5f,     1.f, 1.f,1.f,
	   -0.5f, -0.5f, -0.5f,     1.f, 1.f,1.f,
	   //front face -- green
	   -0.5f, -0.5f,  0.5f,     1.f, 1.f,1.f,
		0.5f, -0.5f,  0.5f,     1.f, 1.f,1.f,
		0.5f,  0.5f,  0.5f,     1.f, 1.f,1.f,
		0.5f,  0.5f,  0.5f,     1.f, 1.f,1.f,
	   -0.5f,  0.5f,  0.5f,     1.f, 1.f,1.f,
	   -0.5f, -0.5f,  0.5f,     1.f, 1.f,1.f,
	   //left face--- blue
	   -0.5f,  0.5f,  0.5f,     1.f, 1.f,1.f,
	   -0.5f,  0.5f, -0.5f,     1.f, 1.f,1.f,
	   -0.5f, -0.5f, -0.5f,     1.f, 1.f,1.f,
	   -0.5f, -0.5f, -0.5f,     1.f, 1.f,1.f,
	   -0.5f, -0.5f,  0.5f,     1.f, 1.f,1.f,
	   -0.5f,  0.5f,  0.5f,     1.f, 1.f,1.f,
	   //right face
		0.5f,  0.5f,  0.5f,     1.f, 1.f,1.f,
		0.5f,  0.5f, -0.5f,     1.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,     1.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,     1.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,     1.0f, 1.0f,1.0f,
		0.5f,  0.5f,  0.5f,     1.0f, 1.0f,1.0f,
		//buttom  
	   -0.5f, -0.5f, -0.5f,     1.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,     1.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,     1.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,     1.0f, 1.0f,1.0f,
	   -0.5f, -0.5f,  0.5f,     1.0f, 1.0f,1.0f,
	   -0.5f, -0.5f, -0.5f,     1.0f, 1.0f,1.0f,
	   //top                    
	   -0.5f,  0.5f, -0.5f,     1.0f, 1.0f,1.0f,
		0.5f,  0.5f, -0.5f,     1.0f, 1.0f,1.0f,
		0.5f,  0.5f,  0.5f,     1.0f, 1.0f,1.0f,
		0.5f,  0.5f,  0.5f,     1.0f, 1.0f,1.0f,
	   -0.5f,  0.5f,  0.5f,     1.0f, 1.0f,1.0f,
	   -0.5f,  0.5f, -0.5f,     1.0f, 1.0f,1.0f
	};
}

