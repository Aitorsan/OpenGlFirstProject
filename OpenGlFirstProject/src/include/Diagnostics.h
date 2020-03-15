#pragma once
#include <Windows.h>
#include <iostream>

#define LOG(message,color) \
SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), color);\
std::cerr << "[LOG] " << message <<'\n';\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)

#define LOG_ERROR(message) \
SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), 12);\
std::cerr << "[ERROR] " << message <<'\n';\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)


#define LOG_DEBUG(message)\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);\
std::cerr << "[DEBUG] " << message <<'\n';\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)

#define LOG_DEBUG_N(message)\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);\
std::cout  << message <<',';\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)

void DebugProgramLinkError(unsigned int shaderProgramId);
void DebugShaderCompileStatus(unsigned int shaderId, const char* shaderType);