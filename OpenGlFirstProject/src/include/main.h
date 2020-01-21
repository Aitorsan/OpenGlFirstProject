#include <vector>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void Transforms(GLuint,bool);
void ChangeTranslations(unsigned int, std::vector<glm::mat4>&stuff);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
GLuint GetSkyBoxTexture(std::vector<std::string>& faces);
GLuint GetTexture(const char* texturePath);
void processInput(float elapsed_time, GLFWwindow* window);
void SkyBoxTransforms(GLuint shaderProgram);
using namespace glm;
#define PRINT \
if (once)\
{\
	std::cout << "\n-----------------\n" << "View matrix\n-------------------\n";\
	for (int i = 0; i < 4; ++i)\
	{\
		for (int u = 0; u < 4; ++u)\
		{\
			std::cout << "    " << cameraTranslation[i][u] << "      ";\
		}\
		std::cout << std::endl;\
	}\
	once = false;\
	std::cout << "-----------------\n" << std::endl;\
}
//Initialization of GlEW and GLFW libraries
void InitGLEW()
{
	glewExperimental = GL_TRUE;
	int err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
}

GLFWwindow* CreateGLFWwindow(int window_width, int window_height, const char* window_title)
{
	GLFWwindow* window = nullptr;
	if (!glfwInit())
		exit(-1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback([](int error, const char* message) {
		std::cerr << "[ Error: " << error << " ] " << message << std::endl;
		});
	window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);
	
	if (!window)
	{
		glfwTerminate();
		exit(-1);
	}
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	InitGLEW();
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) mutable
	{
			glfwGetFramebufferSize(window, &width, &height);
		
			glViewport(0, 0, width, height);
	});
	std::cout << glGetString(GL_VERSION) << std::endl;
	return window;
}