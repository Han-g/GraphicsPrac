#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

#include <iostream>

// ���̴� �ε� �Լ�
char* filetobuf(const char* file);

// ���̴� ���� �Լ�
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();

// ���� ���� ����
extern GLuint vertexShader, fragmentShader, shaderProgram;