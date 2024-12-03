#define ON 1

#if ON == 0

#include "shader.h"

#include <vector>
#include <cstdlib>
#include <ctime>

// 전역 변수
GLuint cubeVAO, cubeVBO;
GLuint sphereVAO, sphereVBO, sphereEBO;
std::vector<unsigned int> sphereIndices;
int sphereIndexCount;

glm::mat4 projection, view;
GLint projLoc, viewLoc, modelLoc;

float rotationAngleY = 0.0f;
float rotationAngleMouse = 0.0f;
int lastMouseX = -1;

struct Sphere {
    glm::vec3 position;
    glm::vec3 velocity;
    float radius;
};

std::vector<Sphere> spheres;
const int MAX_SPHERES = 5;

// 직육면체 색상 (각 면 다른 색)
glm::vec3 cubeColors[6] = {
    glm::vec3(1.0f, 0.0f, 0.0f), // 왼쪽 - 회색
    glm::vec3(0.0f, 1.0f, 0.0f), // 오른쪽 - 초록
    glm::vec3(0.0f, 0.0f, 1.0f), // 아래쪽 - 파랑
    glm::vec3(1.0f, 1.0f, 0.0f), // 위쪽 - 노랑
    glm::vec3(1.0f, 0.0f, 1.0f), // 뒤쪽 - 보라
    glm::vec3(0.0f, 1.0f, 1.0f)  // 앞쪽 - 시안
};

// 직육면체 정점 정의 (안쪽 면)
float cubeVertices[] = {
    -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,// 좌하좌
    -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,// 좌상좌
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,// 좌상우
    -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,// 좌하우

    // 오른쪽
    1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,// 우하좌
    1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,// 우상좌
    1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,// 우상우
    1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,// 우하우

    // 아래쪽
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,

    // 위쪽
    -1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
    1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f,

    // 뒤쪽
    -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,

    // 앞쪽
    -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f
};

// 큐브 인덱스 (각 면을 시계방향으로 돌리기 위해 반대로 순서 변경)
unsigned int cubeIndices[] = {
    // 뒤쪽 면
    0, 1, 2, 0, 2, 3,
    // 앞쪽 면
    4, 5, 6, 4, 6, 7,
    // 왼쪽 면
    8, 9, 10, 8, 10, 11,
    // 오른쪽 면
    12, 13, 14, 12, 14, 15,
    // 아래쪽 면
    16, 17, 18, 16, 18, 19,
    // 위쪽 면
    20, 21, 22, 20, 22, 23
};

// 구 생성 함수 (단순 구)
void createSphereGeometry(float radius, unsigned int sectorCount, unsigned int stackCount)
{
    float x, y, z, xy;                              // vertex position
    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    float stackStep = glm::pi<float>() / stackCount;
    float sectorAngle, stackAngle;

    // 정점 생성
    for (unsigned int i = 0; i <= stackCount; ++i)
    {
        stackAngle = glm::pi<float>() / 2 - i * stackStep;        // 위에서부터 아래쪽으로
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // 각 스택에 대해 섹터를 순회
        for (unsigned int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // 각도 측정
            x = xy * cosf(sectorAngle);             // x 좌표
            y = xy * sinf(sectorAngle);             // y 좌표
            sphereIndices.push_back(i * (sectorCount + 1) + j);
            // 정점 추가
            // 위치 데이터만 저장
            // 실제로는 VBO에 위치 정보를 담아야 함
        }
    }

    // 인덱스 생성
    std::vector<unsigned int> indices;
    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // 현재 스택
        k2 = k1 + sectorCount + 1;      // 다음 스택

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                sphereIndices.push_back(k1);
                sphereIndices.push_back(k2);
                sphereIndices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1))
            {
                sphereIndices.push_back(k1 + 1);
                sphereIndices.push_back(k2);
                sphereIndices.push_back(k2 + 1);
            }
        }
    }

    sphereIndexCount = sphereIndices.size();
}

// 큐브 생성 함수
void createCube()
{
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    GLuint cubeEBO;
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // 위치 속성 (layout(location = 0))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 색상 속성 (layout(location = 1))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// 구 생성 함수
void createSphere()
{
    createSphereGeometry(0.5f, 20, 20);

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    // 단순화를 위해 위치 데이터를 생략하고 셰이더에서 임의의 구를 그리도록 함

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), &sphereIndices[0], GL_STATIC_DRAW);

    // 위치 속성 (layout(location = 0))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 색상 속성 (layout(location = 1))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// 카메라 설정
void setupCamera(int width, int height)
{
    projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    projLoc = glGetUniformLocation(shaderProgram, "projection");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

// 디스플레이 콜백 함수
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // 큐브 그리기
    glBindVertexArray(cubeVAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationAngleMouse), glm::vec3(0.0f, 0.0f, 1.0f));
    modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    for (int i = 0; i < 6; ++i)
    {
        glUniform3f(glGetUniformLocation(shaderProgram, "color"), cubeColors[i].x, cubeColors[i].y, cubeColors[i].z);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int)));
    }
    glBindVertexArray(0);

    glDisable(GL_CULL_FACE);

    //// 구 그리기
    //glBindVertexArray(sphereVAO);
    //for (auto& s : spheres)
    //{
    //    glm::mat4 sphereModel = glm::mat4(1.0f);
    //    sphereModel = glm::translate(sphereModel, s.position);
    //    modelLoc = glGetUniformLocation(shaderProgram, "model");
    //    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel));
    //    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f); // 흰색 공
    //    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    //}
    //glBindVertexArray(0);


    glutSwapBuffers();
}

// 타이머 콜백 함수 (애니메이션 업데이트)
void timer(int)
{
    // 공 위치 업데이트
    for (auto& s : spheres)
    {
        s.position += s.velocity * 0.01f;

        // 벽 충돌 체크 (-1 ~ 1 범위 내)
        if (s.position.x - s.radius < -1.0f || s.position.x + s.radius > 1.0f)
            s.velocity.x = -s.velocity.x;
        if (s.position.y - s.radius < -1.0f || s.position.y + s.radius > 1.0f)
            s.velocity.y = -s.velocity.y;
        if (s.position.z - s.radius < -1.0f || s.position.z + s.radius > 1.0f)
            s.velocity.z = -s.velocity.z;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 약 60 FPS
}

// 키보드 입력 처리
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'z':
        // z축 양 방향 이동
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        break;
    case 'Z':
        // z축 음 방향 이동
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        break;
    case 'y':
        // y축 시계 방향 회전
        rotationAngleY -= 5.0f;
        break;
    case 'Y':
        // y축 반시계 방향 회전
        rotationAngleY += 5.0f;
        break;
    case 'B':
    case 'b':
        if (spheres.size() < MAX_SPHERES)
        {
            Sphere newSphere;
            newSphere.position = glm::vec3(0.0f, 0.0f, 0.0f);
            newSphere.velocity = glm::vec3(
                ((rand() % 200) / 100.0f - 1.0f),
                ((rand() % 200) / 100.0f - 1.0f),
                ((rand() % 200) / 100.0f - 1.0f)
            );
            newSphere.radius = 0.1f;
            spheres.push_back(newSphere);
        }
        break;
    case 27: // ESC 키
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// 마우스 이동 처리
void motion(int x, int y)
{
    if (lastMouseX != -1)
    {
        int dx = x - lastMouseX;
        rotationAngleMouse += dx * 0.5f;
        if (rotationAngleMouse > 60.0f)
            rotationAngleMouse = 60.0f;
        if (rotationAngleMouse < -60.0f)
            rotationAngleMouse = -60.0f;
    }
    lastMouseX = x;
    glutPostRedisplay();
}

// 리사이즈 처리
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    setupCamera(width, height);
}

// 초기화 함수
void init()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW 초기화 실패" << std::endl;
        exit(-1);
    }

    make_vertexShaders();
    make_fragmentShaders();
    make_shaderProgram();

    createCube();
    createSphere();

    glEnable(GL_DEPTH_TEST);

    srand(time(0));
}

// 메인 함수
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D 공 튕기기");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}


#elif ON == 0



#endif