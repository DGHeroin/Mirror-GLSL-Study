#pragma  once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")
#pragma warning(disable : 4996)

GLuint g_program;
 
char* readShaderSource(const char* shaderFile);

int initShader(const GLchar* vShaderFile, const GLchar* fShaderFile);