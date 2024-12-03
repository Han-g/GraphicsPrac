#define ON 0

#if ON == 1

#define _CRT_SECURE_NO_WARNINGS
#include "shader.h"
#include <Windows.h>
#include <time.h>

GLvoid drawScene();
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid SpecialKeyBoard(int key, int x, int y);
GLvoid Reshape(int w, int h);
GLvoid TimerFunc(int x);

void InitBuffer();

float vertexPosition[] = {
    -0.5f,0.5f,0.5f, //앞면
    -0.5f,-0.5f,0.5f,
     0.5f,-0.5f,0.5f,
     0.5f,0.5f,0.5f,

   -0.5f,0.5f,-0.5f, //윗면
  -0.5f,0.5f,0.5f,
   0.5f,0.5f,0.5f,
   0.5f,0.5f,-0.5f,

   -0.5f,0.5f,-0.5f, //왼쪽옆
   -0.5f,-0.5f,-0.5f,
   -0.5f,-0.5f,0.5f,
   -0.5f,0.5f,0.5f,

   0.5f,0.5f,-0.5f, //뒷면
   0.5f,-0.5f,-0.5f,
   -0.5f,-0.5f,-0.5f,
   -0.5f,0.5f,-0.5f,

   -0.5f,-0.5f,0.5f, //아랫면
   -0.5f,-0.5f,-0.5f,
   0.5f,-0.5f,-0.5f,
   0.5f,-0.5f,0.5f,

   0.5f,0.5f,0.5f, //오른쪽 옆
   0.5f,-0.5f,0.5f,
   0.5f,-0.5f,-0.5f,
   0.5f,0.5f,-0.5f,

   2.0f,0,0, //x,y,z 축
   -2.0f,0,0,
   0,2.0f,0,
   0,-2.0f,0,
   0,0,4.0f,
   0,0,-4.0f, //30

   //사면체
    0, 0.5f, 0,
   -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,

    0, 0.5f, 0,
   -0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f, 0.5f,

    0, 0.5f, 0,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,

    0, 0.5f, 0,
    0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,

   -0.5f, -0.5f, 0.5f,
   -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f
};//정육면체, 축,정사면체 벡터들

float vertexColor[] = {
   0,1,1,
   0,0,1,
   1,0,1,
   1,1,1,

   0,1,0,
   0,1,1,
   1,1,1,
   1,1,0,

   0,1,0,
   0,0,0,
   0,0,1,
   0,1,1,

   1,1,0,
   1,0,0,
   0,0,0,
   0,1,0,

   0,0,1,
   0,0,0,
   1,0,0,
   1,0,1,

   1,1,1,
   1,0,1,
   1,0,0,
   1,1,0,


   //선
   0,0,0,
   0,0,0,
   0,0,0,
   0,0,0,
   0,0,0,
   0,0,0,


   //정사면체
   0,1,1,
   0,0,1,
   1,0,1,

   0,1,1,
   1,1,0,
   0,0,1,

   0,1,1,
   1,0,1,
   1,0,0,

   0,1,1,
   1,0,0,
   1,1,0,

    0,0,1,
    1,1,0,
    1,0,0,
    1,0,1,
};//정육면체, 축,정사면체 색깔들

float CubeVertexData[] = {
    // 앞면
    -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,

     // 윗면
     -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,

      // 왼쪽면
      -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,

      // 뒷면
       0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
       0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
      -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
      -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,

      // 아랫면
      -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
       0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,

       // 오른쪽면
        0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f
};

float ConeVertexData[] = {
    // 첫 번째 면
     0.0f,  0.5f,  0.0f,   0.0f,  0.707f,  0.707f,
    -0.5f, -0.5f,  0.5f,   0.0f,  0.707f,  0.707f,
     0.5f, -0.5f,  0.5f,   0.0f,  0.707f,  0.707f,

     // 두 번째 면
      0.0f,  0.5f,  0.0f,  -0.707f,  0.707f,  0.0f,
     -0.5f, -0.5f, -0.5f,  -0.707f,  0.707f,  0.0f,
     -0.5f, -0.5f,  0.5f,  -0.707f,  0.707f,  0.0f,

     // 세 번째 면
      0.0f,  0.5f,  0.0f,   0.707f,  0.707f,  0.0f,
      0.5f, -0.5f,  0.5f,   0.707f,  0.707f,  0.0f,
      0.5f, -0.5f, -0.5f,   0.707f,  0.707f,  0.0f,

      // 네 번째 면
       0.0f,  0.5f,  0.0f,   0.0f,  0.707f, -0.707f,
       0.5f, -0.5f, -0.5f,   0.0f,  0.707f, -0.707f,
      -0.5f, -0.5f, -0.5f,   0.0f,  0.707f, -0.707f
};

GLuint shaderID;

int main(int argc, char** argv)
{
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Example1");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";

    make_vertexShaders();
    make_fragmentShaders();
    shaderID = make_shaderProgram();
    InitBuffer();
    glutKeyboardFunc(KeyBoard);
    glutSpecialFunc(SpecialKeyBoard);
    glutTimerFunc(10, TimerFunc, 1);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

bool lightEnabled = false;

bool hFlag = false;
bool wFlag = true;
bool cFlag = false;

bool xFlag = false;
float xAngle{};

bool yFlag = false;
float yAngle{};

float xTrans{}, yTrans{};

bool depth = false;
GLuint VAO, VBO[2];

void InitBuffer()
{
    glGenVertexArrays(1, &VAO); //--- VAO 를 지정하고 할당하기
    glGenBuffers(2, VBO); //--- 2개의 VBO를 지정하고 할당하기

    glBindVertexArray(VAO); //--- VAO를 바인드하기
    //--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    //--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.

    if (cFlag) {
        // 사면체(Cone) 데이터를 VBO에 복사
        glBufferData(GL_ARRAY_BUFFER, sizeof(ConeVertexData), ConeVertexData, GL_STATIC_DRAW);
    }
    else {
        // 정육면체(Cube) 데이터를 VBO에 복사
        glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertexData), CubeVertexData, GL_STATIC_DRAW);
    }

    // Vertex Position 설정 (attribute 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex Normal 설정 (attribute 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // VAO 바인딩 해제
}

GLvoid drawScene() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    hFlag ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);//은면제거
    wFlag ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//와이어 객체

    unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform");//월드 변환 행렬값을 셰이더의 uniform mat4 modelTransform에게 넘겨줌
    unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");//위와 동일
    unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");//위와 동일

    unsigned int lightPosLoc = glGetUniformLocation(shaderID, "lightPos");
    unsigned int viewPosLoc = glGetUniformLocation(shaderID, "viewPos");
    unsigned int lightColorLoc = glGetUniformLocation(shaderID, "lightColor");
    unsigned int objectColorLoc = glGetUniformLocation(shaderID, "objectColor");

    ////원근 투영
    //glm::mat4 kTransform = glm::mat4(1.0f);
    //kTransform = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
    //kTransform = glm::translate(kTransform, glm::vec3(0.0, 0.0, -5.0f));
    //glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &kTransform[0][0]);

    ////뷰잉 변환
    //glm::mat4 vTransform = glm::mat4(1.0f);
    //glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f); //--- 카메라 위치
    //glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
    //glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

    //vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    //glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &vTransform[0][0]);

    ////축
    //glm::mat4 axisTransForm = glm::mat4(1.0f);//변환 행렬 생성 T
    //axisTransForm = glm::rotate(axisTransForm, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));//x축에 대하여 30도 회전
    //axisTransForm = glm::rotate(axisTransForm, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));//y축에 대하여 30도 회전
    //glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axisTransForm));//변환 행렬을 셰이더에 전달
    //glDrawArrays(GL_LINES, 24, 6);//저장해둔 배열의 24번째 값부터 6개 사용


    //glm::mat4 shapeTransForm = glm::mat4(1.0f);//변환 행렬 생성 T
    //shapeTransForm = glm::translate(shapeTransForm, glm::vec3(xTrans, yTrans, 0.0));//이동
    //shapeTransForm = glm::rotate(shapeTransForm, glm::radians(yAngle), glm::vec3(0.0, 1.0, 0.0));//y축 회전
    //shapeTransForm = glm::rotate(shapeTransForm, glm::radians(xAngle), glm::vec3(1.0, 0.0, 0.0));//x축 회전
    //shapeTransForm = axisTransForm * shapeTransForm;//기존에 축 변환 행렬을 도형의 변환행렬에게 곱해줌으로써 변환들을 상속시킴
    //glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(shapeTransForm));//변환 행렬을 셰이더에 전달

     // 행렬 Uniform
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // 조명 정보
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor = glm::vec3(1.0f, 0.0f, 0.2f);

    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));

    if (cFlag) {
        glDrawArrays(GL_TRIANGLES, 0, 12); // 정사면체: 15개 정점
    }
    else glDrawArrays(GL_QUADS, 0, 24); //정육면체

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

GLvoid KeyBoard(unsigned char key, int x, int y) {

    switch (key) {
    case 'm':
        lightEnabled = !lightEnabled;
        glUniform1i(glGetUniformLocation(shaderID, "lightEnabled"), lightEnabled);
        break;
    case 'h':
        hFlag = 1 - hFlag;
        break;
    case 'w':
        wFlag = !wFlag;
        break;
    case 'c':
        cFlag = false;
        InitBuffer();
        break;
    case 'p':
        cFlag = true;
        InitBuffer();
        break;
    case 'x':
        xFlag = 1 - xFlag;
        break;
    case 'y':
        yFlag = 1 - yFlag;
        break;
    case 's':
        xFlag = 0;
        yFlag = 0;
        xAngle = 0.f;
        yAngle = 0.f;
        yTrans = 0.f;
        xTrans = 0.f;
    default:
        break;

    }
    glutPostRedisplay();
}

GLvoid SpecialKeyBoard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        xTrans = xTrans - 0.1f;
        break;
    case GLUT_KEY_RIGHT:
        xTrans = xTrans + 0.1f;
        break;
    case GLUT_KEY_DOWN:
        yTrans = yTrans - 0.1f;
        break;
    case GLUT_KEY_UP:
        yTrans = yTrans + 0.1f;
        break;
    }
    glutPostRedisplay();
}

GLvoid TimerFunc(int x) {
    switch (x)
    {
    case 1:
        if (xFlag) {
            xAngle = xAngle + 1.f;
            if (xAngle > 360)xAngle = 0.f;
        }
        if (yFlag) {
            yAngle = yAngle + 1.f;
            if (yAngle > 360)yAngle = 0.f;
        }
        break;
    }

    glutTimerFunc(10, TimerFunc, 1);
    glutPostRedisplay();
}

#endif