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

// ���̴� ����
GLuint vertexShader, fragmentShader;
GLuint shaderProgram;

// ���ؽ� �迭 �� ���� ��ü
GLuint VAO_cube, VBO_cube;
GLuint VAO_pyramid, VBO_pyramid;

// ��ȯ ���
glm::mat4 model, view, projection;

// �ִϸ��̼� ���� ����
bool isCulling = false;
bool rotateY = false;
bool rotateCubeTop = false;
bool openCubeFront = false;
bool openCubeSide = false;
bool openCubeBack = false;
bool openPyramid = false;
bool rotatePyramidFaces = false;
bool perspectiveProjection = true;

// ���� ����
float angleY = 0.0f;
float angleCubeTop = 0.0f;
float angleCubeFront = 0.0f;
float angleCubeSide = 0.0f;
float angleCubeBack = 0.0f;
float anglePyramid = 0.0f;

// ������ ���۷� �д� �Լ�
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

// ���ؽ� ���̴� ���� �Լ�
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
        std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
        return;
    }

    free(vertexSource);
}

// �����׸�Ʈ ���̴� ���� �Լ�
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
        std::cerr << "ERROR: frag_shader ������ ����\n" << errorLog << std::endl;
        return;
    }

    free(fragmentSource);
}

// ���̴� ���α׷� ���� �Լ�
GLuint make_shaderProgram()
{
    GLuint shaderID = glCreateProgram(); //--- ���̴� ���α׷� �����
    GLint result;
    GLchar errorLog[512];

    glAttachShader(shaderID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
    glAttachShader(shaderID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
    glLinkProgram(shaderID); //--- ���̴� ���α׷� ��ũ�ϱ�
    glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ�����Ƿ� ���� ����
    glDeleteShader(fragmentShader);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // --- ���̴��� �� ����Ǿ����� üũ�ϱ�

    if (!result) {
        glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
        return false;
    }

    glUseProgram(shaderID);
    return shaderID;
}

// ť�� ���� ������
GLfloat cubeVertices[] = {
    // ��ġ            // ����
    // �ո�
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // ����
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    // �޸�
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // �ʷ�
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    // ���ʸ�
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // �Ķ�
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    // �����ʸ�
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // ���
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     // ����
     -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // ��ȫ
      0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     // �Ʒ���
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // û��
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
};

// ť�� �ε��� ������
GLuint cubeIndices[] = {
    0, 1, 2,  2, 3, 0,        // �ո�
    4, 5, 6,  6, 7, 4,        // �޸�
    8, 9,10, 10,11, 8,        // ���ʸ�
   12,13,14, 14,15,12,        // �����ʸ�
   16,17,18, 18,19,16,        // ����
   20,21,22, 22,23,20         // �Ʒ���
};

// �Ƕ�̵� ���� ������
GLfloat pyramidVertices[] = {
    // ��ġ            // ����
    // �ٴ�
    -0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f, // ȸ��
     0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,
    // ����
     0.0f, 1.0f,  0.0f,  1.0f, 0.5f, 0.0f // ��Ȳ
};

// �Ƕ�̵� �ε��� ������
GLuint pyramidIndices[] = {
    // �ٴ�
    0, 1, 2,
    2, 3, 0,
    // ����
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

// �Լ� ����
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void timer(int value);
void init();

// Ű���� �Է� ó�� �Լ�
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
    case 27: // ESC Ű
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

// �ʱ�ȭ �Լ�
void init()
{
    // GLEW �ʱ�ȭ
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW �ʱ�ȭ ����" << std::endl;
        exit(EXIT_FAILURE);
    }

    // ���̴� ����
    make_vertexShaders();
    make_fragmentShaders();
    shaderProgram = make_shaderProgram();

    // ť�� VAO �� VBO ����
    glGenVertexArrays(1, &VAO_cube);
    glGenBuffers(1, &VBO_cube);
    GLuint EBO_cube;
    glGenBuffers(1, &EBO_cube);

    glBindVertexArray(VAO_cube);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cube);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // ��ġ �Ӽ�
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // ���� �Ӽ�
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // �Ƕ�̵� VAO �� VBO ����
    glGenVertexArrays(1, &VAO_pyramid);
    glGenBuffers(1, &VBO_pyramid);
    GLuint EBO_pyramid;
    glGenBuffers(1, &EBO_pyramid);

    glBindVertexArray(VAO_pyramid);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_pyramid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_pyramid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);

    // ��ġ �Ӽ�
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // ���� �Ӽ�
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // ���� �׽�Ʈ Ȱ��ȭ
    glEnable(GL_DEPTH_TEST);
}

// ���÷��� �Լ�
void display()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ���̴� ���
    glUseProgram(shaderProgram);

    // ī�޶� ����
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -3.0f));

    // �������� ����
    if (perspectiveProjection)
        projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    else
        projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 0.1f, 100.0f);

    // �� ��� ����
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // �������� ��� ����
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // ť�� �� ���
    model = glm::mat4(1.0f);
    if (rotateY)
        angleY += 0.5f;
    model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // ť�� �׸���
    glBindVertexArray(VAO_cube);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // �Ƕ�̵� �� ���
    glm::mat4 model_pyramid = glm::mat4(1.0f);
    model_pyramid = glm::translate(model_pyramid, glm::vec3(1.5f, 0.0f, 0.0f));
    if (rotatePyramidFaces)
        anglePyramid += 1.0f;
    model_pyramid = glm::rotate(model_pyramid, glm::radians(anglePyramid), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_pyramid));

    // �Ƕ�̵� �׸���
    glBindVertexArray(VAO_pyramid);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glutSwapBuffers();
}

// �������� �Լ�
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
}

// Ÿ�̸� �Լ�
void timer(int value)
{
    // �ִϸ��̼� ������Ʈ ���� �߰� ����
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // �� 60 FPS
}

int main(int argc, char** argv)
{
    // FreeGLUT �ʱ�ȭ
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("����ü�� �簢�� �ִϸ��̼�");

    // �ʱ�ȭ �Լ� ȣ��
    init();

    // �ݹ� �Լ� ����
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    // ���� ���� ����
    glutMainLoop();

    return 0;
}

#endif