#define ON 0

#if ON == 1

// main.cpp
#include "shader.h"

#include <cstdlib>
#include <cstring>

// 카메라 관련 변수
glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 25.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.3f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// 투영 및 뷰 행렬
glm::mat4 projection;
glm::mat4 view;

// 조명 관련 변수
glm::vec3 lightColors[3] = {
    glm::vec3(1.0f, 1.0f, 1.0f), // 흰색
    glm::vec3(1.0f, 0.0f, 0.0f), // 빨간색
    glm::vec3(0.0f, 0.0f, 1.0f)  // 파란색
};
glm::vec3 lightPos = glm::vec3(1.0f, 10.0f, 10.0f); // 조명의 초기 위치
glm::vec3 lightColor = lightColors[0]; // 조명의 초기 색상
bool lightEnabled = true; // 조명 활성화 여부
bool lightOrbit = false; // 조명의 공전 여부
float lightOrbitAngle = 0.0f; // 공전 각도
int lightColorIndex = 0; // 조명 색상 변경을 위한 인덱스


// 크레인 관련 변수
bool isAnimating = false;
float turretAngle = 0.0f;   // 포신의 회전 각도
float cameraAngleY = 0.0f;  // 카메라의 y축 회전 각도
float orbitAngle = 0.0f;    // 화면 중심에 대한 카메라 공전 각도
bool isTurretSeparated = true; // 포신이 분리된 상태

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

// 카메라 애니메이션
bool cameraAutoRotate = false;
float cameraOrbitAngle = 0.0f;
float angle = 0.0f;

// VAO 및 VBO
GLuint VAO, VBO;

// 정육면체의 정점 데이터
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

// 초기화 함수
void init()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW 초기화 실패" << std::endl;
        exit(EXIT_FAILURE);
    }

    make_vertexShaders();
    make_fragmentShaders();
    shaderProgram = make_shaderProgram();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // VAO 바인딩
    glBindVertexArray(VAO);

    // VBO 바인딩 및 데이터 설정
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 위치 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 색상 속성
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);



    glBindVertexArray(0); // VAO 언바인딩

    // 투영 행렬 설정
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
}

// 디스플레이 콜백 함수
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram);

    // Uniform: Light Enable
    GLuint lightEnableLoc = glGetUniformLocation(shaderProgram, "lightEnabled");
    glUniform1i(lightEnableLoc, lightEnabled); // 조명 활성화

    // Uniform: Light Position
    GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3f(lightPosLoc, lightPos[0], lightPos[1], lightPos[2]);

    // Uniform: View Position (카메라 위치)
    GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    glUniform3f(viewPosLoc, cameraPos[0], cameraPos[1], cameraPos[2]);

    // Uniform: Light Color
    GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3f(lightColorLoc, lightColor[0], lightColor[1], lightColor[2]);

    // Uniform: Object Color
    GLuint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);

    // 뷰 행렬 설정
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // 투영 행렬 설정
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);

    // 바닥 그리기
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f));
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 크레인 그리기
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(basePosX, 0.55f, 0.0f));
    // 아래 몸체
    glm::mat4 base = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(base));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 중앙 몸체
    glm::mat4 central = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
    central = glm::rotate(central, glm::radians(centralRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    central = glm::scale(central, glm::vec3(1.5f, 1.0f, 1.5f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(central));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 포신 1
    glm::mat4 gun1 = glm::translate(central, glm::vec3(1.0f, 0.0f, 0.0f));
    gun1 = glm::rotate(gun1, glm::radians(gunRotation1), glm::vec3(0.0f, 1.0f, 0.0f));
    gun1 = glm::scale(gun1, glm::vec3(0.5f, 0.2f, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gun1));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 포신 2
    glm::mat4 gun2 = glm::translate(central, glm::vec3(-1.0f, 0.0f, 0.0f));
    gun2 = glm::rotate(gun2, glm::radians(gunRotation2), glm::vec3(0.0f, 1.0f, 0.0f));
    gun2 = glm::scale(gun2, glm::vec3(0.5f, 0.2f, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gun2));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 팔 1
    glm::mat4 arm1 = glm::translate(central, glm::vec3(0.0f, 1.0f, 0.0f));
    arm1 = glm::rotate(arm1, glm::radians(armRotation1), glm::vec3(0.0f, 0.0f, 1.0f));
    arm1 = glm::translate(arm1, glm::vec3(0.0f, 0.0f, 1.5f));
    arm1 = glm::scale(arm1, glm::vec3(1.0f, 0.2f, 3.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(arm1));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 팔 2
    glm::mat4 arm2 = glm::translate(central, glm::vec3(0.0f, 1.0f, 0.0f));
    arm2 = glm::rotate(arm2, glm::radians(armRotation2), glm::vec3(0.0f, 0.0f, -1.0f));
    arm2 = glm::translate(arm2, glm::vec3(0.0f, 0.0f, -1.5f));
    arm2 = glm::scale(arm2, glm::vec3(1.0f, 0.2f, 3.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(arm2));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 조명
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos); // 조명 위치로 이동
    modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));

    glutSolidSphere(0.5f, 20, 20);

    glBindVertexArray(0);

    glutSwapBuffers();
}

// 타이머 콜백 함수
void timer(int value)
{
    if (lightOrbit) {
        lightOrbitAngle += 1.0f; // 공전 속도
        if (lightOrbitAngle >= 360.0f) lightOrbitAngle -= 360.0f;

        lightPos.x = 10.f * cos(glm::radians(lightOrbitAngle));
        lightPos.z = 10.f * sin(glm::radians(lightOrbitAngle));
        lightPos.y = 10.0f;
    }

    // 아래 몸체 이동
    if (moveBasePositive)
    {
        basePosX += 0.05f;
    }
    if (moveBaseNegative)
    {
        basePosX -= 0.05f;
    }

    // 중앙 몸체 회전
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

    // 포신 회전
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

    // 팔 회전
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

    // 카메라 자동 회전
    if (cameraAutoRotate)
    {
        cameraOrbitAngle += 0.5f;
        if (cameraOrbitAngle > 360.0f) cameraOrbitAngle -= 360.0f;
        cameraPos.x = 15.0f * sin(glm::radians(cameraOrbitAngle));
        cameraPos.z = 15.0f * cos(glm::radians(cameraOrbitAngle));
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 약 60 FPS
}

void toggleTurretSeparation() {
    if (isTurretSeparated) {
        // 포신을 합치는 애니메이션
        turretAngle = 0.0f; // 회전 상태가 있을 경우 원래 상태로 초기화
        isTurretSeparated = false;
    }
    else {
        // 포신을 다시 분리하는 애니메이션
        isTurretSeparated = true;
    }
}


void animateOrbit() {
    if (isAnimating) {
        orbitAngle += 0.5f; // 공전 속도 조절
        if (orbitAngle >= 360.0f) orbitAngle = 0.0f;
    }
}

// 키보드 입력 처리 함수
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
    case 'M':
        lightEnabled = !lightEnabled;
        init();
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
    case 'c': // 조명 색상 변경
        lightColorIndex = (lightColorIndex + 1) % 3;
        lightColor = lightColors[lightColorIndex];
        break;

    case 'y': // 조명 공전 시작
        lightOrbit = true;
        break;

    case 's': // 조명 공전 멈추기
        lightOrbit = false;
        break;
    case 'Q':
    case 'q':
        exit(0);
        break;
    default:
        break;
    }
}

// 메인 함수
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("이동하는 크레인");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}

#endif