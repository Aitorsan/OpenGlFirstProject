#include <../third-party/include/GL/glew.h>
#include <../third-party/include/GLFW/glfw3.h>
#include <../third-party/include/glm/glm.hpp>
#include <../third-party/include/glm/gtc/matrix_transform.hpp>
#include <../third-party/include/sphere/Sphere.h>
#define STB_IMAGE_IMPLEMENTATION
#include <../third-party/include/stb_image.h>
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include "include/Shaders.h"
#include "include/Maths.hpp"
#include "include/main.h"

//evil globals lol
 int SCR_WIDTH = 800;
 int SCR_HEIGHT = 600;

//camera data 
float cameraX = 0.0f,cameraY = 0.0f, cameraZ = 2.f;
glm::vec3 cameraFront{ 0,0,-1.0f };
glm::vec3 camerapos(cameraX, cameraY, cameraZ);
glm::vec3 cameraUp(0, 1, 0);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = SCR_WIDTH /2.0f;
float lastY = SCR_HEIGHT /2.0f ;
constexpr float fov = 45.0f;

//thread stuff
std::mutex mut;

//light color
glm::vec3 lightColor(1.9f,1.9f,1.9f);
glm::vec3 lightPosition(0.0f,0.0f,-0.5f);

//projeciton matrix can be precomputed 
 glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)(SCR_HEIGHT), 0.5f, 10000.f);

////////////////// MAIN ////////////////////////////////////////////////////////////////////////
int main()
{   
	GLFWwindow* window = CreateGLFWwindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Aitor");
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);

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
	//cubes
	float vertices[] = {
	   -0.5f, -0.5f, -0.5f,	   0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,	   0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,	   0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 	    1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,	   0.0f, 0.0f, -1.0f,	    0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,	    0.0f, 0.0f,

	   -0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    0.0f, 0.0f,

	   -0.5f,  0.5f,  0.5f,	   -1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,	   -1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,	   -1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,	   -1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,	   -1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,	   -1.0f,  0.0f,  0.0f,	    1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,	    0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f,	    0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,	    0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,     0.0f, 1.0f
	};
	
	//light is a sphere written by a third party
	Sphere lightSphere;
	

	/*************************************
	* Shader programs
	**************************************/
	GLuint shaderProgram = CreateShaderProgram("shaders/vertex.shader", "shaders/fragment.shader");
	GLuint lightShaderProgram = CreateShaderProgram("shaders/lightVertex.shader","shaders/lightFragment.shader");
	GLuint skyBoxShaderProgram = CreateShaderProgram("shaders/skyBoxVertex.shader", "shaders/skyBoxFragment.shader");

	/*************************************
	* Light buffer objects
	**************************************/
	GLuint lightVAO, lightVBO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, lightSphere.getVertexSize(), lightSphere.vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
	glBindVertexArray(0);

	/*************************************
	* Skybox buffer objects
	**************************************/
	GLuint skyBoxVAO, skyBoxVBO;
	glGenVertexArrays(1, &skyBoxVAO);
	glGenBuffers(1, &skyBoxVBO);
	glBindVertexArray(skyBoxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxVertices), &skyBoxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::vector<std::string> boxFaces
	{
		"textures/lightBlueSpace/right.png",
		"textures/lightBlueSpace/left.png",
		"textures/lightBlueSpace/top.png",
		"textures/lightBlueSpace/bot.png",
		"textures/lightBlueSpace/front.png",
		"textures/lightBlueSpace/back.png"
	};

	GLuint skyBoxTexture = GetSkyBoxTexture(boxFaces);
	
	/*************************************
	* Cubes buffer objects
	**************************************/
	unsigned int VAO, VBO;
	//Vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//Vertex buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	//position data layout
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), 0);
	
	//normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(3*sizeof(float)));
	//texture data 
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Instancing buffer for the cubes
	const unsigned int amount = 10000;
	std::vector<glm::mat4> model_array;
	model_array.reserve(amount);
	// Send the transformation matrices
	unsigned int  vb_instance;
	glGenBuffers(1, &vb_instance);
	glBindBuffer(GL_ARRAY_BUFFER, vb_instance);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	GLsizei vec4Size = sizeof(glm::vec4);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	
	std::thread loop_thread{ &ChangeTranslations, amount, std::ref(model_array) };
	loop_thread.detach();
	
	glDisable(GL_CULL_FACE);
	//Texture for the cubes
	GLuint texture = GetTexture("textures/rock.jpg");

	float last = glfwGetTime();
	
	int colorFactor = 0;
	while (!glfwWindowShouldClose(window))
	{   
		float current = glfwGetTime();
		float elapsed = current - last;
		last = current;
		processInput(elapsed,window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Draw skybox
		glUseProgram(skyBoxShaderProgram);
		Transforms(skyBoxShaderProgram, true);
		mat4 skyModel(1.0f);
		GLuint skyModelLoc = glGetUniformLocation(skyBoxShaderProgram, "model");
		glUniformMatrix4fv(skyModelLoc, 1, GL_FALSE, &skyModel[0][0]);
		glBindVertexArray(skyBoxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// draw light source
		glUseProgram(lightShaderProgram);
		GLuint locationlightColor = glGetUniformLocation(lightShaderProgram, "ligthColor");
		glUniform3fv(locationlightColor, 1, &lightColor[0]);
		Transforms(lightShaderProgram,false);
		mat4 model(1.0f);
		model = glm::scale(model, vec3(9, 9, 9.0f));
		model = glm::translate(model, lightPosition);
		GLuint modelLocation = glGetUniformLocation(lightShaderProgram, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);
		GLuint lightColorLocation = glGetUniformLocation(lightShaderProgram, "lightColor");
		glUniform3fv(lightColorLocation, 1, &lightColor[0]);
		glBindVertexArray(lightVAO);
		lightSphere.draw();

		//draw cubes
		glUseProgram(shaderProgram);
		Transforms(shaderProgram, false);
		GLuint camerPosLocation = glGetUniformLocation(shaderProgram, "viewPos");
		glUniform3fv(shaderProgram, 1, &camerapos[0]);
		GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
		glUniform3fv(lightColorLoc, 1, &lightColor[0]);
		GLuint lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
		glUniform3fv(lightPosLocation, 1, &lightPosition[0]);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		mut.lock();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4)*model_array.size(), &model_array[0]);
		std::size_t currsize = model_array.size();
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, currsize);
		mut.unlock();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &skyBoxVAO);
	glDeleteBuffers(1, &lightVAO);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void Transforms(GLuint shaderProgram,bool skybox = false)
{
	static bool once = true;
	glm::mat4 cameraTranslation;
	if(!skybox)
		cameraTranslation = glm::lookAt(camerapos ,camerapos + cameraFront, glm::vec3(0.0, 1.0, 0.0));
	else
		cameraTranslation = mat4(mat3(glm::lookAt(camerapos, camerapos + cameraFront, glm::vec3(0.0, 1.0, 0.0))));
	PRINT
	GLint cameraTrans = glGetUniformLocation(shaderProgram, "camera");
	glUniformMatrix4fv(cameraTrans, 1, GL_FALSE, &cameraTranslation[0][0]);

	GLint projectionLocation = glGetUniformLocation(shaderProgram, "project");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}


void ChangeTranslations(unsigned int amount ,std::vector<glm::mat4>& model_array)
{
	for(unsigned long i = 0; i <amount ;++i)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(float(i)), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::translate(model, glm::vec3(cos(glfwGetTime()*i)*200,sin(glfwGetTime())*10,tanf(glfwGetTime()*i)));
		mut.lock();
		model_array.push_back( model);
		mut.unlock();	
	} 
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	static bool fistMouse = true;
	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f; 
	
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw))* cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw))* cos(glm::radians(pitch));
	
	cameraFront = glm::normalize(direction);
}


GLuint GetSkyBoxTexture(std::vector<std::string>& faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); ++i)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	return textureID;
}
GLuint GetTexture(const char* texturePath)
{
	//read the image file for the texture
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	// generate a texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return texture;
}

void processInput(float elapsed_time,GLFWwindow* window)
{
    float velocity = 10.f;
	
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		velocity += 300.f;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true); 
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camerapos  += cameraFront*velocity*elapsed_time;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camerapos -=  cameraFront* velocity*elapsed_time;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camerapos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * elapsed_time*velocity;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{

		camerapos += glm::normalize(glm::cross(cameraFront, cameraUp)) * elapsed_time*velocity;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camerapos.y += 1.5f* elapsed_time;
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	SCR_WIDTH = width;
	SCR_HEIGHT = height;

	projectionMatrix = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)(SCR_HEIGHT), 0.5f, 10000.f);
	glViewport(0, 0, width, height);
}
