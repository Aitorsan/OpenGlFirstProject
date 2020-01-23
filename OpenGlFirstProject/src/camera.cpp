#pragma once
#include "include/camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
Camera::Camera()
    :Camera(0.0f, 0.0f, 2.0f, 45.0f)
{
}

Camera::Camera(float x, float y, float z, float fov)
	: CameraPos{x,y,z}
	, CameraFront{ 0, 0,-1.0f }
	, CameraUp{ 0, 1, 0 }
	, yaw{-90}
	, pitch{0.0f}
	, lastX{x}
	, lastY{y}
{

}

void Camera::CheckMouseMovement(GLFWwindow& window)
{   
	double xpos, ypos;
	glfwGetCursorPos(&window, &xpos, &ypos);
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

	CameraFront = glm::normalize(direction);
}

void Camera::MoveFront(float elapsedTime, float velocity)
{
	CameraPos += CameraFront * velocity*elapsedTime;
}

void Camera::MoveBack(float elapsedTime, float velocity)
{
	CameraPos -= CameraFront * velocity*elapsedTime;
}

void Camera::MoveRight(float elapsedTime, float velocity)
{
	CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * elapsedTime*velocity;
}

void Camera::MoveLeft(float elapsedTime, float velocity)
{
	CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * elapsedTime*velocity;
}

glm::mat4 Camera::GetCameraTranslationMatrix()
{
	return glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

glm::vec3& Camera::GetCameraPosition()
{
	return CameraPos;
}

void Camera::SetCameraPosition(float x, float y, float z)
{
	CameraPos.x = x;
	CameraPos.y = y;
	CameraPos.z = z;
}
