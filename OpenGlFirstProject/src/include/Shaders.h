#include <tuple>

unsigned int CreateShaderProgram(const char* vertexShaderSourcePath, const char* fragmentShaderSourcePath);
static std::tuple<unsigned int, unsigned int> GetShadersID(const char* vertexShaderSourcePath, const char* fragmentShaderSourcePath);
