#define ON 0

#if ON == 1

// main.cpp
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <cstring>

// ���̴� ���� ���� ����
GLuint vertexShader, fragmentShader, shaderProgram;
int width = 800, height = 600;

// ���̴� ���α׷��� �����ϴ� �Լ���
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
        std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
        return;
    }
    free(fragmentSource);
}

GLuint make_shaderProgram()
{
    GLuint shaderID = glCreateProgram(); //--- ���̴� ���α׷� �����
    GLint result;
    GLchar errorLog[512];

    glAttachShader(shaderID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
    glAttachShader(shaderID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
    glLinkProgram(shaderID); //--- ���̴� ���α׷� ��ũ�ϱ�
    glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
    glDeleteShader(fragmentShader);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�

    if (!result) {
        glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
        return 0;
    }

    glUseProgram(shaderID);
    return shaderID;
}

// ī�޶� ���� ����
glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 15.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.3f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// ���� �� �� ���
glm::mat4 projection;
glm::mat4 view;

// ũ���� ���� ����
bool isAnimating = false;
float turretAngle = 0.0f;   // ������ ȸ�� ����
float cameraAngleY = 0.0f;  // ī�޶��� y�� ȸ�� ����
float orbitAngle = 0.0f;    // ȭ�� �߽ɿ� ���� ī�޶� ���� ����
bool isTurretSeparated = true; // ������ �и��� ����

float basePosX = 0.0f;
bool moveBasePositive = false;
bool moveBaseNegative = false;

float centralRotation = 0.0f;
bool rotateCentralPositive = false;
bool rotateCentralNegative = false;

float gunRotation1 = 0.0f;
float gunRotation2 = 0.0f;
bool rotateGunsPositive = false;
bool rotateGunsNegative = false;

bool gunsMerged = false;

float armRotation1 = 0.0f;
float armRotation2 = 0.0f;
bool rotateArmsPositive = false;
bool rotateArmsNegative = false;

// ī�޶� �ִϸ��̼�
bool cameraAutoRotate = false;
float cameraOrbitAngle = 0.0f;
float angle = 0.0f;

// VAO �� VBO
GLuint VAO, VBO;

// ������ü�� ���� ������
GLfloat vertices[] = {
    // Positions          // Colors
    -0.5f, -0.5f, -0.5f,  0.583f,  0.771f,  0.014f,
     0.5f, -0.5f, -0.5f,  0.609f,  0.115f,  0.436f,
     0.5f,  0.5f, -0.5f,  0.327f,  0.483f,  0.844f,
     0.5f,  0.5f, -0.5f,  0.822f,  0.569f,  0.201f,
    -0.5f,  0.5f, -0.5f,  0.435f,  0.602f,  0.223f,
    -0.5f, -0.5f, -0.5f,  0.310f,  0.747f,  0.185f,

    -0.5f, -0.5f,  0.5f,  0.597f,  0.770f,  0.761f,
     0.5f, -0.5f,  0.5f,  0.559f,  0.436f,  0.730f,
     0.5f,  0.5f,  0.5f,  0.359f,  0.583f,  0.152f,
     0.5f,  0.5f,  0.5f,  0.483f,  0.596f,  0.789f,
    -0.5f,  0.5f,  0.5f,  0.559f,  0.861f,  0.639f,
    -0.5f, -0.5f,  0.5f,  0.195f,  0.548f,  0.859f,

    -0.5f,  0.5f,  0.5f,  0.014f,  0.184f,  0.576f,
    -0.5f,  0.5f, -0.5f,  0.771f,  0.328f,  0.970f,
    -0.5f, -0.5f, -0.5f,  0.406f,  0.615f,  0.116f,
    -0.5f, -0.5f, -0.5f,  0.676f,  0.977f,  0.133f,
    -0.5f, -0.5f,  0.5f,  0.971f,  0.572f,  0.833f,
    -0.5f,  0.5f,  0.5f,  0.140f,  0.616f,  0.489f,

     0.5f,  0.5f,  0.5f,  0.997f,  0.513f,  0.064f,
     0.5f,  0.5f, -0.5f,  0.945f,  0.719f,  0.592f,
     0.5f, -0.5f, -0.5f,  0.543f,  0.021f,  0.978f,
     0.5f, -0.5f, -0.5f,  0.279f,  0.317f,  0.505f,
     0.5f, -0.5f,  0.5f,  0.167f,  0.620f,  0.077f,
     0.5f,  0.5f,  0.5f,  0.347f,  0.857f,  0.137f,

    -0.5f, -0.5f, -0.5f,  0.055f,  0.953f,  0.042f,
     0.5f, -0.5f, -0.5f,  0.714f,  0.505f,  0.345f,
     0.5f, -0.5f,  0.5f,  0.783f,  0.290f,  0.734f,
     0.5f, -0.5f,  0.5f,  0.722f,  0.645f,  0.174f,
    -0.5f, -0.5f,  0.5f,  0.302f,  0.455f,  0.848f,
    -0.5f, -0.5f, -0.5f,  0.225f,  0.587f,  0.040f,

    -0.5f,  0.5f, -0.5f,  0.517f,  0.713f,  0.338f,
     0.5f,  0.5f, -0.5f,  0.053f,  0.959f,  0.120f,
     0.5f,  0.5f,  0.5f,  0.393f,  0.621f,  0.362f,
     0.5f,  0.5f,  0.5f,  0.673f,  0.211f,  0.457f,
    -0.5f,  0.5f,  0.5f,  0.820f,  0.883f,  0.371f,
    -0.5f,  0.5f, -0.5f,  0.982f,  0.099f,  0.879f
};

// �ʱ�ȭ �Լ�
void init()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW �ʱ�ȭ ����" << std::endl;
        exit(EXIT_FAILURE);
    }

    make_vertexShaders();
    make_fragmentShaders();
    shaderProgram = make_shaderProgram();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // VAO ���ε�
    glBindVertexArray(VAO);

    // VBO ���ε� �� ������ ����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ��ġ �Ӽ�
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // ���� �Ӽ�
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // VAO ����ε�

    // ���� ��� ����
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
}

// ���÷��� �ݹ� �Լ�
void renderScene() {
    glBindVertexArray(VAO);

    // �ٴ� �׸���
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f));
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ũ���� �׸���
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(basePosX, 0.55f, 0.0f));
    // �Ʒ� ��ü
    glm::mat4 base = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(base));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // �߾� ��ü
    glm::mat4 central = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
    central = glm::rotate(central, glm::radians(centralRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    central = glm::scale(central, glm::vec3(1.5f, 1.0f, 1.5f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(central));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ���� 1
    glm::mat4 gun1 = glm::translate(central, glm::vec3(1.0f, 0.0f, 0.0f));
    gun1 = glm::rotate(gun1, glm::radians(gunRotation1), glm::vec3(0.0f, 1.0f, 0.0f));
    gun1 = glm::scale(gun1, glm::vec3(0.5f, 0.2f, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gun1));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ���� 2
    glm::mat4 gun2 = glm::translate(central, glm::vec3(-1.0f, 0.0f, 0.0f));
    gun2 = glm::rotate(gun2, glm::radians(gunRotation2), glm::vec3(0.0f, 1.0f, 0.0f));
    gun2 = glm::scale(gun2, glm::vec3(0.5f, 0.2f, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gun2));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // �� 1
    glm::mat4 arm1 = glm::translate(central, glm::vec3(0.0f, 1.0f, 0.0f));
    arm1 = glm::rotate(arm1, glm::radians(armRotation1), glm::vec3(0.0f, 0.0f, 1.0f));
    arm1 = glm::translate(arm1, glm::vec3(0.0f, 0.0f, 1.5f));
    arm1 = glm::scale(arm1, glm::vec3(1.0f, 0.2f, 3.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(arm1));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // �� 2
    glm::mat4 arm2 = glm::translate(central, glm::vec3(0.0f, 1.0f, 0.0f));
    arm2 = glm::rotate(arm2, glm::radians(armRotation2), glm::vec3(0.0f, 0.0f, -1.0f));
    arm2 = glm::translate(arm2, glm::vec3(0.0f, 0.0f, -1.5f));
    arm2 = glm::scale(arm2, glm::vec3(1.0f, 0.2f, 3.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(arm2));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram);

    // �� ��� ����
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");

    // ���� ��� ����
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    // ù ��° ����Ʈ: ���� ����
    glViewport(0, 0, width / 3, height);
    glm::mat4 perspectiveProjection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(perspectiveProjection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    renderScene(); // ����� �׸��� �Լ�

    // �� ��° ����Ʈ: ���� ���� (xz ���)
    glViewport(width / 3, 0, width / 3, height);
    glm::mat4 orthoProjectionXZ = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(orthoProjectionXZ));
    glm::mat4 xzView = glm::lookAt(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(xzView));
    renderScene();

    // �� ��° ����Ʈ: ���� ���� (xy ���)
    glViewport(2 * width / 3, 0, width / 3, height);
    glm::mat4 orthoProjectionXY = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(orthoProjectionXY));
    glm::mat4 xyView = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(xyView));
    renderScene();

    glutSwapBuffers();
}

// Ÿ�̸� �ݹ� �Լ�
void timer(int value)
{
    // �Ʒ� ��ü �̵�
    if (moveBasePositive)
    {
        basePosX += 0.05f;
    }
    if (moveBaseNegative)
    {
        basePosX -= 0.05f;
    }

    // �߾� ��ü ȸ��
    if (rotateCentralPositive)
    {
        centralRotation += 1.0f;
        if (centralRotation > 90.0f) centralRotation = 90.0f;
    }
    if (rotateCentralNegative)
    {
        centralRotation -= 1.0f;
        if (centralRotation < -90.0f) centralRotation = -90.0f;
    }

    // ���� ȸ��
    if (rotateGunsPositive)
    {
        gunRotation1 += 1.0f;
        gunRotation2 -= 1.0f;
        if (gunRotation1 > 90.0f) { gunRotation1 = 90.0f; gunRotation2 = -90.0f; }
    }
    if (rotateGunsNegative)
    {
        gunRotation1 -= 1.0f;
        gunRotation2 += 1.0f;
        if (gunRotation1 < -90.0f) { gunRotation1 = -90.0f; gunRotation2 = 90.0f; }
    }

    // �� ȸ��
    if (rotateArmsPositive)
    {
        armRotation1 += 1.0f;
        armRotation2 -= 1.0f;
        if (armRotation1 > 90.0f) { armRotation1 = 90.0f; armRotation2 = -90.0f; }
    }
    if (rotateArmsNegative)
    {
        armRotation1 -= 1.0f;
        armRotation2 += 1.0f;
        if (armRotation1 < -90.0f) { armRotation1 = -90.0f; armRotation2 = 90.0f; }
    }

    // ī�޶� �ڵ� ȸ��
    if (cameraAutoRotate)
    {
        cameraOrbitAngle += 0.5f;
        if (cameraOrbitAngle > 360.0f) cameraOrbitAngle -= 360.0f;
        cameraPos.x = 15.0f * sin(glm::radians(cameraOrbitAngle));
        cameraPos.z = 15.0f * cos(glm::radians(cameraOrbitAngle));
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // �� 60 FPS
}

void toggleTurretSeparation() {
    if (isTurretSeparated) {
        // ������ ��ġ�� �ִϸ��̼�
        turretAngle = 0.0f; // ȸ�� ���°� ���� ��� ���� ���·� �ʱ�ȭ
        isTurretSeparated = false;
    }
    else {
        // ������ �ٽ� �и��ϴ� �ִϸ��̼�
        isTurretSeparated = true;
    }
}


void animateOrbit() {
    if (isAnimating) {
        orbitAngle += 0.5f; // ���� �ӵ� ����
        if (orbitAngle >= 360.0f) orbitAngle = 0.0f;
    }
}

// Ű���� �Է� ó�� �Լ�
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'b':
        moveBasePositive = false;
        break;
    case 'B':
        moveBasePositive = !moveBasePositive;
        moveBaseNegative = false;
        break;
    case 'm':
        rotateCentralPositive = false;
        break;
    case 'M':
        rotateCentralPositive = !rotateCentralPositive;
        rotateCentralNegative = false;
        break;
    case 'f':
        rotateGunsPositive = false;
        break;
    case 'F':
        rotateGunsPositive = !rotateGunsPositive;
        rotateGunsNegative = !rotateGunsPositive;
        break;
    case 'e':
    case 'E':
        toggleTurretSeparation();
        break;
    case 't':
        rotateArmsPositive = false;
        break;
    case 'T':
        rotateArmsPositive = !rotateArmsPositive;
        rotateArmsNegative = !rotateArmsPositive;
        break;
    case 'z':
        cameraPos.z -= 1.0f;
        break;
    case 'Z':
        cameraPos.z += 1.0f;
        break;
    case 'x':
        cameraPos.x -= 1.0f;
        break;
    case 'X':
        cameraPos.x += 1.0f;
        break;
    case 'y':
    case 'Y':
        angle = glm::radians(1.0f); // ȸ�� ����(1�� ������ ȸ�� ����)
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        cameraPos = glm::vec3(rotation * glm::vec4(cameraPos, 1.0f));
        break;
    case 'r':
    case 'R':
        cameraOrbitAngle += 0.01f; // �����ϴ� ����
        cameraPos.x = 15.0f * sin(cameraOrbitAngle); // �ݰ� ����
        cameraPos.z = 15.0f * cos(cameraOrbitAngle); // �ݰ� ����
        break;
    case 'a':
    case 'A':
        isAnimating = !isAnimating;
        break;
    case 's':
    case 'S':
        // ��� ������ ���߱�
        moveBasePositive = moveBaseNegative = false;
        rotateCentralPositive = rotateCentralNegative = false;
        rotateGunsPositive = rotateGunsNegative = false;
        rotateArmsPositive = rotateArmsNegative = false;
        cameraAutoRotate = false;
        break;
    case 'c':
    case 'C':
        // ��� ������ �ʱ�ȭ
        cameraOrbitAngle = 0.f; angle = 0.f;
        moveBasePositive = moveBaseNegative = false;
        rotateCentralPositive = rotateCentralNegative = false;
        rotateGunsPositive = rotateGunsNegative = false;
        rotateArmsPositive = rotateArmsNegative = false;
        cameraAutoRotate = false;
        basePosX = 0.0f;
        centralRotation = 0.0f;
        gunRotation1 = gunRotation2 = 0.0f;
        armRotation1 = armRotation2 = 0.0f;
        cameraPos = glm::vec3(0.0f, 5.0f, 15.0f);
        cameraFront = glm::vec3(0.0f, -0.3f, -1.0f);
        break;
    case 'Q':
    case 'q':
        exit(0);
        break;
    default:
        break;
    }
}

// ���� �Լ�
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("�̵��ϴ� ũ����");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}

#endif