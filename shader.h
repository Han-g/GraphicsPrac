#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

#include <iostream>

// 쉐이더 로딩 함수
char* filetobuf(const char* file);

// 쉐이더 생성 함수
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();

// 전역 변수 선언
extern GLuint vertexShader, fragmentShader, shaderProgram;