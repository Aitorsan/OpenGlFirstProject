

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shapes/Sphere.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <future>
#include <chrono>
#include "include/Shaders.h"
#include "include/Maths.hpp"
#include "include/main.h"
#include "include/Cubes.h"
#include "include/camera.h"
#include "include/WFObjLoader.h"
#include "include/utilities.hpp"
#include "include/Diagnostics.h"

GLuint DRAW_TYPE= GL_FILL;

//thread stuff
std::mutex mut;

//projeciton matrix can be precomputed 
constexpr float fov = 45.0f;
glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)(SCR_HEIGHT), 0.5f, 10000.f);

////////////////// MAIN ////////////////////////////////////////////////////////////////////////
int main()                           
{   
	GLFWwindow* window = CreateGLFWwindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Aitor");
	Camera camera(0.0f,0.0f,100.0f,45.f);
	//sky box
	float skyBoxVertices[108]{};
	genSkyBoxCube(skyBoxVertices);
	//Cube shape
	constexpr int cubeSize = 192, indSize = 36;
	float cube[cubeSize]{};
	unsigned int cubeIndices[indSize]{};
	genCubeWithTextureCoordsAndIndices(cube,cubeIndices);
	//light is a sphere written by a third party
	Sphere lightSphere;
		
	//light position
	glm::vec3 lightPosition(0.0f,0.0f,-0.5f);
	//light color
	glm::vec3 lightColor(0.9f, 0.9f, 0.9f);

	/*************************************
	* Shader programs
	**************************************/
	GLuint shaderProgram = CreateShaderProgram("shaders/vertex.shader", "shaders/fragment.shader");
	GLuint lightShaderProgram = CreateShaderProgram("shaders/lightVertex.shader","shaders/lightFragment.shader");
	GLuint skyBoxShaderProgram = CreateShaderProgram("shaders/skyBoxVertex.shader", "shaders/skyBoxFragment.shader");
	GLuint modelShaderProgram = CreateShaderProgram("shaders/modelVertex.shader", "shaders/modelFragment.shader");
	/**********************************
	* Object loader shape
	***********************************/
	std::string data = ReadObjFile("3dModels/Bugatti.txt");
	WFObjLoader f(data,WFObjLoader::GEN_INDICES);
	auto rawModel = f.GetModelDataForIndexing();
	auto indicesf = f.GetIndices();

/*
	std::ofstream mofile("model.txt");
	if (!mofile.is_open())
		return -1;

	for (int i = 0; i < rawModel.size();++i)
	{
		mofile << rawModel[i];
		if ((i+1)% 8 == 0 && i != 0)
			mofile << std::endl;
		else
			mofile << " , ";
	}
	mofile << std::endl << "-------------\n indices\n----------------" << std::endl;
	for (int i = 0; i < indicesf.size(); ++i)
	{
		mofile << indicesf[i];
		if ((i + 1) % 8 == 0 && i != 0)
			mofile << std::endl;
		else
			mofile << " , ";
	}*/

	GLuint shapeVAO, shapeVBO,shapeEBO;
	glGenVertexArrays(1, &shapeVAO);
	glGenBuffers(1, &shapeVBO);
	glGenBuffers(1, &shapeEBO);
	glBindVertexArray(shapeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
	glBufferData(GL_ARRAY_BUFFER, rawModel.size()*sizeof(float), rawModel.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shapeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesf.size() * sizeof(GLuint), indicesf.data(), GL_STATIC_DRAW);

	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_TRUE, 8* sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	
	glBindVertexArray(0);

	/*************************************
	* Light buffer objects
	**************************************/
	GLuint lightVAO, lightVBO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);
	glBindVertexArray(lightVAO);
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
		"textures/redSpace/bkg2_right1.png",
		"textures/redSpace/bkg2_left2.png",
		"textures/redSpace/bkg2_top3.png",
		"textures/redSpace/bkg2_bottom4.png",
		"textures/redSpace/bkg2_front5.png",
		"textures/redSpace/bkg2_back6.png"
	};

	GLuint skyBoxTexture = GetSkyBoxTexture(boxFaces);

	/*************************************
	* Cubes buffer objects
	**************************************/
	unsigned int VAO, VBO,EBO;
	//Vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Vertex buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_DYNAMIC_DRAW);
	//Element array buffer
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_DYNAMIC_DRAW);
	//position data layout
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), 0);
	//texture data 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(6*sizeof(float)));

    // Instancing buffer for the cubes
	const unsigned int amount = 10000;
	std::vector<glm::mat4> model_array;
	model_array.reserve(amount);
	// Send the transformation matrices
	unsigned int  vb_instance;
	glGenBuffers(1, &vb_instance);
	glBindBuffer(GL_ARRAY_BUFFER, vb_instance);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	unsigned long long vec4Size = sizeof(glm::vec4);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4u * vec4Size, (void*)(2u * vec4Size));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	//Texture for the cubes
	GLuint texture = GetTexture("textures/rock.jpg");

	std::thread loop_thread{ &ChangeTranslations, amount, std::ref(model_array) };
	loop_thread.detach();

	float last{0};
	float velocity = 10.f;
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{   

		float current = glfwGetTime();
		float elapsed = current - last;
		last = current;
		
		processInput(window,camera,elapsed,velocity);
		glClearColor(0.5f, 0.7f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Draw skybox
		glUseProgram(skyBoxShaderProgram);
		Transforms(skyBoxShaderProgram,camera, true);
		glm::mat4 skyModel(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyBoxShaderProgram, "model"), 1, GL_FALSE, &skyModel[0][0]);
		glBindVertexArray(skyBoxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);
		glDisable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, DRAW_TYPE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glEnable(GL_DEPTH_TEST);

		// draw light source
		glUseProgram(lightShaderProgram);
		glUniform3fv(glGetUniformLocation(lightShaderProgram, "ligthColor"), 1, &lightColor[0]);
		Transforms(lightShaderProgram,camera,false);
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(9, 9, 9.0f));
		model = glm::translate(model, lightPosition);
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
		glUniform3fv(glGetUniformLocation(lightShaderProgram, "lightColor"), 1, &lightColor[0]);
		glBindVertexArray(lightVAO);
		lightSphere.draw(DRAW_TYPE);

		//draw cubes
		glUseProgram(shaderProgram);
		Transforms(shaderProgram, camera,false);
		glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, &camera.GetCameraPosition()[0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor") , 1, &lightColor[0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, &lightPosition[0]);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		mut.lock();
		std::size_t currsize = model_array.size();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4)*currsize, &model_array[0]);
		mut.unlock();
		glPolygonMode(GL_FRONT_AND_BACK, DRAW_TYPE);
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, currsize);
		
        //DRAW last stuff
		glUseProgram(modelShaderProgram);
		Transforms(modelShaderProgram,camera, false);

		glm::mat4 modelobj = glm::mat4(1.0f);
		auto times = (float)glfwGetTime();
		//modelobj = glm::rotate(modelobj,glm::radians(times)*20, glm::vec3(0.f, 1.f, 0.f));
		modelobj = glm::translate(modelobj, glm::vec3(0,30, 0));

		glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "model"), 1, GL_FALSE, &modelobj[0][0]);
		glBindVertexArray(shapeVAO);
		glPolygonMode(GL_FRONT_AND_BACK, DRAW_TYPE);
		//glDrawArrays(GL_TRIANGLES, 0,9);
		//modelobj = glm::rotate(modelobj, times, glm::vec3(0.f, 2.0f, 0.f));
		//modelobj = glm::translate(modelobj, glm::vec3(2.0f, 0.0f, cos(times/2.0f) * 6.0));
		glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "model"), 1, GL_FALSE, &modelobj[0][0]);
		glPolygonMode(GL_FRONT_AND_BACK, DRAW_TYPE);
		//glDrawArrays(GL_TRIANGLES, 0, rawModel.size());
		glDrawElements(GL_TRIANGLES, indicesf.size(), GL_UNSIGNED_INT, 0);


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

void Transforms(GLuint shaderProgram, Camera& camera , bool skybox = false)
{
	glm::mat4 cameraTranslation = camera.GetCameraTranslationMatrix();
	if(skybox)
		cameraTranslation = glm::mat4(glm::mat3(cameraTranslation));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "camera"), 1, GL_FALSE, &cameraTranslation[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "project"), 1, GL_FALSE, &projectionMatrix[0][0]);
}

void ChangeTranslations(unsigned int amount ,std::vector<glm::mat4>& model_array)
{
	for(unsigned long i = 0; i <amount ;++i)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(float(glfwGetTime())), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::translate(model, glm::vec3(sin(203.0f*i/400)*400,cos(301.0f*i/400)*401,sin(400.0f*i/600)*405));
		mut.lock();
		model_array.push_back( model);
		mut.unlock();	
	} 
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

void processInput(GLFWwindow* window,Camera& camera ,float elapsedTime,float velocity)
{
	camera.CheckMouseMovement(*window);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		DRAW_TYPE = GL_LINE;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		DRAW_TYPE = GL_FILL;
	}
	
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		velocity += 10.5f;
	}
	if(glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
	{
		if (velocity > 20.f)
			velocity -= 0.2f;
	}
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true); 
	}
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.MoveFront(elapsedTime, velocity);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.MoveBack(elapsedTime, velocity);
	}
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.MoveLeft(elapsedTime, velocity);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.MoveRight(elapsedTime, velocity);
	}
	
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

