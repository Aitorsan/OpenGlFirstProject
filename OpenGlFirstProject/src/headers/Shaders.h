
 struct Shaders
{ 
	unsigned int VertexID;
	unsigned int FragmentID;
};

unsigned int CreateShaderProgram(const char* vertexShaderSourcePath, const char* fragmentShaderSourcePath);
static Shaders GetShadersID(const char* vertexShaderSourcePath, const char* fragmentShaderSourcePath);
