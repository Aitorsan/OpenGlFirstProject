#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "headers/Shaders.h"
#include "headers/Maths.hpp"

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
float translationFactor = -3.0f;
float translationXFactor = 0.0f;
float rotXCamera = 0.0f;
float rotYCamera = 0.0f;
float rotZCamera = 0.0f;
float angleRot = 30.f * PI / 180.f;
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
	glfwMakeContextCurrent(window);
	InitGLEW();
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) mutable
	{

		glfwGetFramebufferSize(window, &width, &height);
		SCR_WIDTH = width;
		SCR_HEIGHT = height;
		glViewport(0, 0, width, height);
	});
	std::cout << glGetString(GL_VERSION) << std::endl;
	return window;
}


void processInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		translationFactor += 0.01f;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		translationFactor -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		translationXFactor += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		translationXFactor -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		rotXCamera += 0.01f;
	
	}
		
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotXCamera -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		rotZCamera += 0.01f;

	}

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		rotZCamera -= 0.01f;
	}


	std::cout << '\r'<<"x: "<<translationXFactor << ",y:0" <<",z: "<<translationFactor<<"| xRot: " << rotXCamera << ", yRot: " << rotYCamera << ", zRot: "<<rotZCamera;
}
////////////////// MAIN ////////////////////////////////////////////////////////////////////////
int main()
{
	GLFWwindow* window = CreateGLFWwindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Aitor");
	
	//create shader programs
	GLuint shaderProgram = CreateShaderProgram("shaders/vertex.shader", "shaders/fragment.shader");
    
	glEnable(GL_DEPTH_TEST);
	//Create geometry
	float cube[] = 
	{
		//back face
	   -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, 1.0f,1.0f,
		0.5f,  0.5f, -0.5f,     0.0f, 1.0f,1.0f,
		0.5f,  0.5f, -0.5f,     0.0f, 1.0f,1.0f,
	   -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,1.0f,
	   -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,1.0f,
	
	   //front face
	   -0.5f, -0.5f,  0.5f,     0.0f, 1.0f,0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, 1.0f,0.0f,
		0.5f,  0.5f,  0.5f,     0.0f, 1.0f,0.0f,
		0.5f,  0.5f,  0.5f,     0.0f, 1.0f,0.0f,
	   -0.5f,  0.5f,  0.5f,     0.0f, 1.0f,0.0f,
	   -0.5f, -0.5f,  0.5f,     0.0f, 1.0f,0.0f,
		
	   //left face
	   -0.5f,  0.5f,  0.5f,     0.0f, 0.0f,1.0f,
	   -0.5f,  0.5f, -0.5f,     0.0f, 0.0f,1.0f,
	   -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,1.0f,
	   -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,1.0f,
	   -0.5f, -0.5f,  0.5f,     0.0f, 1.0f,1.0f,
	   -0.5f,  0.5f,  0.5f,     0.0f, 1.0f,1.0f,

	   //right face
		0.5f,  0.5f,  0.5f,     1.0f, 0.0f,1.0f,
		0.5f,  0.5f, -0.5f,     1.0f, 0.0f,1.0f,
		0.5f, -0.5f, -0.5f,     1.0f, 0.0f,1.0f,
		0.5f, -0.5f, -0.5f,     1.0f, 0.0f,1.0f,
		0.5f, -0.5f,  0.5f,     1.0f, 0.0f,1.0f,
		0.5f,  0.5f,  0.5f,     1.0f, 0.0f,1.0f,
							    
	   -0.5f, -0.5f, -0.5f,     0.5f, 0.5f,1.0f,
		0.5f, -0.5f, -0.5f,     0.5f, 0.5f,1.0f,
		0.5f, -0.5f,  0.5f,     0.5f, 0.5f,1.0f,
		0.5f, -0.5f,  0.5f,     0.5f, 0.5f,1.0f,
	   -0.5f, -0.5f,  0.5f,     0.5f, 0.5f,1.0f,
	   -0.5f, -0.5f, -0.5f,     1.0f, 1.0f,1.0f,
		
	   //top
	   -0.5f,  0.5f, -0.5f,     1.0f, 0.0f,0.0f,
		0.5f,  0.5f, -0.5f,     1.0f, 0.0f,0.0f,
		0.5f,  0.5f,  0.5f,     1.0f, 0.0f,0.0f,
		0.5f,  0.5f,  0.5f,     1.0f, 0.0f,0.0f,
	   -0.5f,  0.5f,  0.5f,     1.0f, 0.0f,0.0f,
	   -0.5f,  0.5f, -0.5f,     1.0f, 0.0f,0.0f
	};

	
	//create buffer objects
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_DYNAMIC_DRAW);

	//position data layout
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), 0);
	//color data 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));

	//use shader program
	glUseProgram(shaderProgram);
   
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// transforms

		float scaleMatrix[4][4];
		//rotation 
		float translationMatrix[4][4];
		float projectionMatrix[4][4];
		
		math::CreateScaleMatrix(scaleMatrix, 1.0f, 10.0f,1.f );
		math::CreateTranslationMatrix(translationMatrix, translationXFactor,0, translationFactor);
		math::CreateProjectionMatrix(projectionMatrix, (float)SCR_WIDTH, (float)SCR_HEIGHT);
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation = glm::rotate(rotation,angleRot, glm::vec3(rotXCamera, 1,rotZCamera));
		
		GLint scaleLocation = glGetUniformLocation(shaderProgram, "scale");
		glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, &scaleMatrix[0][0]);

		GLint rotationLocation = glGetUniformLocation(shaderProgram, "rotate");
		glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, &rotation[0][0]);
		
		GLint translationLocation = glGetUniformLocation(shaderProgram, "translate");
		glUniformMatrix4fv(translationLocation, 1, GL_FALSE, &translationMatrix[0][0]);
	
		GLint projectionLocation = glGetUniformLocation(shaderProgram, "project");
		glUniformMatrix4fv(projectionLocation, 1,GL_FALSE, &projectionMatrix[0][0]);
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}