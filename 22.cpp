#define ON 1

#if ON == 0

#include "shader.h"

#include <vector>
#include <cstdlib>
#include <ctime>

// ���� ����
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

// ������ü ���� (�� �� �ٸ� ��)
glm::vec3 cubeColors[6] = {
    glm::vec3(1.0f, 0.0f, 0.0f), // ���� - ȸ��
    glm::vec3(0.0f, 1.0f, 0.0f), // ������ - �ʷ�
    glm::vec3(0.0f, 0.0f, 1.0f), // �Ʒ��� - �Ķ�
    glm::vec3(1.0f, 1.0f, 0.0f), // ���� - ���
    glm::vec3(1.0f, 0.0f, 1.0f), // ���� - ����
    glm::vec3(0.0f, 1.0f, 1.0f)  // ���� - �þ�
};

// ������ü ���� ���� (���� ��)
float cubeVertices[] = {
    -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,// ������
    -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,// �»���
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,// �»��
    -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,// ���Ͽ�

    // ������
    1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,// ������
    1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,// �����
    1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,// ����
    1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,// ���Ͽ�

    // �Ʒ���
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,

    // ����
    -1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
    1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f,

    // ����
    -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,

    // ����
    -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f
};

// ť�� �ε��� (�� ���� �ð�������� ������ ���� �ݴ�� ���� ����)
unsigned int cubeIndices[] = {
    // ���� ��
    0, 1, 2, 0, 2, 3,
    // ���� ��
    4, 5, 6, 4, 6, 7,
    // ���� ��
    8, 9, 10, 8, 10, 11,
    // ������ ��
    12, 13, 14, 12, 14, 15,
    // �Ʒ��� ��
    16, 17, 18, 16, 18, 19,
    // ���� ��
    20, 21, 22, 20, 22, 23
};

// �� ���� �Լ� (�ܼ� ��)
void createSphereGeometry(float radius, unsigned int sectorCount, unsigned int stackCount)
{
    float x, y, z, xy;                              // vertex position
    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    float stackStep = glm::pi<float>() / stackCount;
    float sectorAngle, stackAngle;

    // ���� ����
    for (unsigned int i = 0; i <= stackCount; ++i)
    {
        stackAngle = glm::pi<float>() / 2 - i * stackStep;        // ���������� �Ʒ�������
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // �� ���ÿ� ���� ���͸� ��ȸ
        for (unsigned int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // ���� ����
            x = xy * cosf(sectorAngle);             // x ��ǥ
            y = xy * sinf(sectorAngle);             // y ��ǥ
            sphereIndices.push_back(i * (sectorCount + 1) + j);
            // ���� �߰�
            // ��ġ �����͸� ����
            // �����δ� VBO�� ��ġ ������ ��ƾ� ��
        }
    }

    // �ε��� ����
    std::vector<unsigned int> indices;
    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // ���� ����
        k2 = k1 + sectorCount + 1;      // ���� ����

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

// ť�� ���� �Լ�
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

    // ��ġ �Ӽ� (layout(location = 0))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ���� �Ӽ� (layout(location = 1))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// �� ���� �Լ�
void createSphere()
{
    createSphereGeometry(0.5f, 20, 20);

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    // �ܼ�ȭ�� ���� ��ġ �����͸� �����ϰ� ���̴����� ������ ���� �׸����� ��

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), &sphereIndices[0], GL_STATIC_DRAW);

    // ��ġ �Ӽ� (layout(location = 0))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ���� �Ӽ� (layout(location = 1))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// ī�޶� ����
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

// ���÷��� �ݹ� �Լ�
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // ť�� �׸���
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

    //// �� �׸���
    //glBindVertexArray(sphereVAO);
    //for (auto& s : spheres)
    //{
    //    glm::mat4 sphereModel = glm::mat4(1.0f);
    //    sphereModel = glm::translate(sphereModel, s.position);
    //    modelLoc = glGetUniformLocation(shaderProgram, "model");
    //    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel));
    //    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f); // ��� ��
    //    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    //}
    //glBindVertexArray(0);


    glutSwapBuffers();
}

// Ÿ�̸� �ݹ� �Լ� (�ִϸ��̼� ������Ʈ)
void timer(int)
{
    // �� ��ġ ������Ʈ
    for (auto& s : spheres)
    {
        s.position += s.velocity * 0.01f;

        // �� �浹 üũ (-1 ~ 1 ���� ��)
        if (s.position.x - s.radius < -1.0f || s.position.x + s.radius > 1.0f)
            s.velocity.x = -s.velocity.x;
        if (s.position.y - s.radius < -1.0f || s.position.y + s.radius > 1.0f)
            s.velocity.y = -s.velocity.y;
        if (s.position.z - s.radius < -1.0f || s.position.z + s.radius > 1.0f)
            s.velocity.z = -s.velocity.z;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // �� 60 FPS
}

// Ű���� �Է� ó��
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'z':
        // z�� �� ���� �̵�
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        break;
    case 'Z':
        // z�� �� ���� �̵�
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        break;
    case 'y':
        // y�� �ð� ���� ȸ��
        rotationAngleY -= 5.0f;
        break;
    case 'Y':
        // y�� �ݽð� ���� ȸ��
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
    case 27: // ESC Ű
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// ���콺 �̵� ó��
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

// �������� ó��
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    setupCamera(width, height);
}

// �ʱ�ȭ �Լ�
void init()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW �ʱ�ȭ ����" << std::endl;
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

// ���� �Լ�
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D �� ƨ���");

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