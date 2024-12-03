#define ON 0

#if ON == 1

// main.cpp
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

// 셰이더 변수
GLuint vertexShader, fragmentShader;
GLuint shaderProgram;

// 버텍스 배열 및 버퍼 객체
GLuint VAO_cube, VBO_cube;
GLuint VAO_pyramid, VBO_pyramid;

// 변환 행렬
glm::mat4 model, view, projection;

// 애니메이션 상태 변수
bool isCulling = false;
bool rotateY = false;
bool rotateCubeTop = false;
bool openCubeFront = false;
bool openCubeSide = false;
bool openCubeBack = false;
bool openPyramid = false;
bool rotatePyramidFaces = false;
bool perspectiveProjection = true;

// 각도 변수
float angleY = 0.0f;
float angleCubeTop = 0.0f;
float angleCubeFront = 0.0f;
float angleCubeSide = 0.0f;
float angleCubeBack = 0.0f;
float anglePyramid = 0.0f;

// 파일을 버퍼로 읽는 함수
char* filetobuf(const char* file)
{
    FILE* fptr;
    long length;
    char* buf;

    if (fopen_s(&fptr, file, "rb") != 0) {
        return NULL;
    }

    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;

    return buf;
}

// 버텍스 셰이더 생성 함수
void make_vertexShaders()
{
    GLchar* vertexSource;
    vertexSource = filetobuf("vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }

    free(vertexSource);
}

// 프래그먼트 셰이더 생성 함수
void make_fragmentShaders()
{
    GLchar* fragmentSource;
    fragmentSource = filetobuf("fragment.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }

    free(fragmentSource);
}

// 셰이더 프로그램 생성 함수
GLuint make_shaderProgram()
{
    GLuint shaderID = glCreateProgram(); //--- 셰이더 프로그램 만들기
    GLint result;
    GLchar errorLog[512];

    glAttachShader(shaderID, vertexShader); //--- 셰이더 프로그램에 버텍스 셰이더 붙이기
    glAttachShader(shaderID, fragmentShader); //--- 셰이더 프로그램에 프래그먼트 셰이더 붙이기
    glLinkProgram(shaderID); //--- 셰이더 프로그램 링크하기
    glDeleteShader(vertexShader); //--- 셰이더 객체를 셰이더 프로그램에 링크했으므로 삭제 가능
    glDeleteShader(fragmentShader);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // --- 셰이더가 잘 연결되었는지 체크하기

    if (!result) {
        glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
        return false;
    }

    glUseProgram(shaderID);
    return shaderID;
}

// 큐브 정점 데이터
GLfloat cubeVertices[] = {
    // 위치            // 색상
    // 앞면
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // 빨강
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    // 뒷면
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // 초록
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    // 왼쪽면
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // 파랑
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    // 오른쪽면
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // 노랑
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     // 윗면
     -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // 자홍
      0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     // 아랫면
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // 청록
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
};

// 큐브 인덱스 데이터
GLuint cubeIndices[] = {
    0, 1, 2,  2, 3, 0,        // 앞면
    4, 5, 6,  6, 7, 4,        // 뒷면
    8, 9,10, 10,11, 8,        // 왼쪽면
   12,13,14, 14,15,12,        // 오른쪽면
   16,17,18, 18,19,16,        // 윗면
   20,21,22, 22,23,20         // 아랫면
};

// 피라미드 정점 데이터
GLfloat pyramidVertices[] = {
    // 위치            // 색상
    // 바닥
    -0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f, // 회색
     0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,
    // 정점
     0.0f, 1.0f,  0.0f,  1.0f, 0.5f, 0.0f // 주황
};

// 피라미드 인덱스 데이터
GLuint pyramidIndices[] = {
    // 바닥
    0, 1, 2,
    2, 3, 0,
    // 옆면
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

// 함수 선언
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void timer(int value);
void init();

// 키보드 입력 처리 함수
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'h':
        isCulling = !isCulling;
        if (isCulling)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
        break;
    case 'y':
        rotateY = !rotateY;
        break;
    case 't':
        rotateCubeTop = !rotateCubeTop;
        break;
    case 'f':
        openCubeFront = !openCubeFront;
        break;
    case 's':
        openCubeSide = !openCubeSide;
        break;
    case 'b':
        openCubeBack = !openCubeBack;
        break;
    case 'o':
        openPyramid = !openPyramid;
        break;
    case 'R':
        rotatePyramidFaces = !rotatePyramidFaces;
        break;
    case 'p':
        perspectiveProjection = !perspectiveProjection;
        break;
    case 27: // ESC 키
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

// 초기화 함수
void init()
{
    // GLEW 초기화
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW 초기화 실패" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 셰이더 생성
    make_vertexShaders();
    make_fragmentShaders();
    shaderProgram = make_shaderProgram();

    // 큐브 VAO 및 VBO 설정
    glGenVertexArrays(1, &VAO_cube);
    glGenBuffers(1, &VBO_cube);
    GLuint EBO_cube;
    glGenBuffers(1, &EBO_cube);

    glBindVertexArray(VAO_cube);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cube);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // 위치 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 색상 속성
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // 피라미드 VAO 및 VBO 설정
    glGenVertexArrays(1, &VAO_pyramid);
    glGenBuffers(1, &VBO_pyramid);
    GLuint EBO_pyramid;
    glGenBuffers(1, &EBO_pyramid);

    glBindVertexArray(VAO_pyramid);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_pyramid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_pyramid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);

    // 위치 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 색상 속성
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // 깊이 테스트 활성화
    glEnable(GL_DEPTH_TEST);
}

// 디스플레이 함수
void display()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 셰이더 사용
    glUseProgram(shaderProgram);

    // 카메라 설정
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -3.0f));

    // 프로젝션 설정
    if (perspectiveProjection)
        projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    else
        projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 0.1f, 100.0f);

    // 뷰 행렬 전송
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // 프로젝션 행렬 전송
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // 큐브 모델 행렬
    model = glm::mat4(1.0f);
    if (rotateY)
        angleY += 0.5f;
    model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // 큐브 그리기
    glBindVertexArray(VAO_cube);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 피라미드 모델 행렬
    glm::mat4 model_pyramid = glm::mat4(1.0f);
    model_pyramid = glm::translate(model_pyramid, glm::vec3(1.5f, 0.0f, 0.0f));
    if (rotatePyramidFaces)
        anglePyramid += 1.0f;
    model_pyramid = glm::rotate(model_pyramid, glm::radians(anglePyramid), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_pyramid));

    // 피라미드 그리기
    glBindVertexArray(VAO_pyramid);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glutSwapBuffers();
}

// 리쉐이프 함수
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
}

// 타이머 함수
void timer(int value)
{
    // 애니메이션 업데이트 로직 추가 가능
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 약 60 FPS
}

int main(int argc, char** argv)
{
    // FreeGLUT 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("육면체와 사각뿔 애니메이션");

    // 초기화 함수 호출
    init();

    // 콜백 함수 설정
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    // 메인 루프 시작
    glutMainLoop();

    return 0;
}

#endif