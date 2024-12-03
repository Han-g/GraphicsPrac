#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#define ON 0

#if ON == 1

struct Triangle {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> color;
    float dx, dy;  // �ﰢ���� �̵� �ӵ�
    int moveType;  // �̵� ����
    int dir = 0;
    float length = 1.f;
};

std::vector<Triangle> triangles;
bool wireframe = false;
int movePattern = 1;  // �⺻ �̵� ����: ƨ���

void addTriangle(float x, float y) {
    if (triangles.size() >= 4) return;

    GLfloat size = (rand() % 5 + 10) / 100.0f;
    GLfloat height = size;
    GLfloat base = size * 2.0f;

    GLfloat colorR = rand() % 256 / 255.0f;
    GLfloat colorG = rand() % 256 / 255.0f;
    GLfloat colorB = rand() % 256 / 255.0f;

    std::vector<GLfloat> vertices = {
        x, y - height,
        x - base / 2, y + height,
        x + base / 2, y + height
    };

    std::vector<GLfloat> color = { colorR, colorG, colorB };

    // �ﰢ���� �ʱ� �ӵ��� �̵� ���� ����
    float dx = (rand() % 5 + 2) / 1000.0f;  // �ӵ� (����)
    float dy = (rand() % 5 + 2) / 1000.0f;  // �ӵ� (����)
    Triangle triangle = { vertices, color, dx, dy, movePattern };
    triangles.push_back(triangle);
}

void drawTriangle(const Triangle& triangle) {
    if (wireframe)
        glBegin(GL_LINE_LOOP);
    else
        glBegin(GL_TRIANGLES);

    glColor3f(triangle.color[0], triangle.color[1], triangle.color[2]);
    glVertex2f(triangle.vertices[0], triangle.vertices[1]);
    glVertex2f(triangle.vertices[2], triangle.vertices[3]);
    glVertex2f(triangle.vertices[4], triangle.vertices[5]);

    glEnd();
}

void initTriangle() {
    addTriangle(rand() % 50 / 100.0f, rand() % 50 / 100.0f);  // 1��и�
    addTriangle(-(rand() % 50 / 100.0f), rand() % 50 / 100.0f);  // 2��и�
    addTriangle(-(rand() % 50 / 100.0f), -(rand() % 50 / 100.0f));  // 3��и�
    addTriangle(rand() % 50 / 100.0f, -(rand() % 50 / 100.0f));  // 4��и�
}

void updateTriangle(Triangle& triangle) {
    // �ﰢ�� �̵� ���Ͽ� ���� ������Ʈ
    
    switch (triangle.moveType) {
    case 1:  // ƨ���
        triangle.vertices[0] += triangle.dx;
        triangle.vertices[1] += triangle.dy;
        triangle.vertices[2] += triangle.dx;
        triangle.vertices[3] += triangle.dy;
        triangle.vertices[4] += triangle.dx;
        triangle.vertices[5] += triangle.dy;

        // ȭ�� ��迡 �ε����� ���� ����
        if (triangle.vertices[0] > 1.0f || triangle.vertices[0] < -1.0f) triangle.dx = -triangle.dx;
        if (triangle.vertices[1] > 1.0f || triangle.vertices[1] < -1.0f) triangle.dy = -triangle.dy;
        break;

    case 2:  // �¿� �������
        triangle.vertices[0] += triangle.dx;
        triangle.vertices[2] += triangle.dx;
        triangle.vertices[4] += triangle.dx;
        if (triangle.vertices[0] > 1.0f || triangle.vertices[0] < -1.0f) triangle.dx = -triangle.dx;
        break;

    case 3:  // �簢�� �����̷�
        if (triangle.dir == 0) {  // ���������� �̵�
            triangle.vertices[0] += triangle.dx;
            triangle.vertices[2] += triangle.dx;
            triangle.vertices[4] += triangle.dx;

            if (triangle.vertices[0] > triangle.length || triangle.vertices[2] < triangle.length * -1) {
                triangle.dx = -triangle.dx;  // ���� ����
                triangle.dir = 1;  // ���� �̵�
                std::cout << triangle.dir << std::endl;
            }
        }

        else if (triangle.dir == 1) {  // ���� �̵�
            triangle.vertices[1] += triangle.dy;
            triangle.vertices[3] += triangle.dy;
            triangle.vertices[5] += triangle.dy;

            if (triangle.vertices[5] > triangle.length || triangle.vertices[1] < triangle.length * -1) {
                triangle.dy = -triangle.dy;  // ���� ����
                triangle.dir = 2;  // �������� �̵�
                std::cout << triangle.dir << std::endl;
            }
        }

        if (triangle.dir == 2) {  // �������� �̵�
            triangle.vertices[0] += triangle.dx;
            triangle.vertices[2] += triangle.dx;
            triangle.vertices[4] += triangle.dx;

            if (triangle.vertices[0] > triangle.length || triangle.vertices[2] < triangle.length * -1) {
                triangle.dx = -triangle.dx;  // ���� ����
                triangle.dir = 3;  // �Ʒ��� �̵�
                std::cout << triangle.dir << std::endl;
            }
        }

        else if (triangle.dir == 3) {  // �Ʒ��� �̵�
            triangle.vertices[1] += triangle.dy;
            triangle.vertices[3] += triangle.dy;
            triangle.vertices[5] += triangle.dy;

            if (triangle.vertices[5] > triangle.length || triangle.vertices[1] < triangle.length * -1) {
                triangle.dy = -triangle.dy;  // ���� ����
                triangle.dir = 0;  // �ٽ� ���������� �̵�
                std::cout << triangle.dir << std::endl;
            }
        }

        break;

    case 4:  // ���� �����̷�
        for (int i = 0; i < 6; i += 2) {
            GLfloat x = triangle.vertices[i];
            GLfloat y = triangle.vertices[i + 1];
            GLfloat angle = 0.02f;
            triangle.vertices[i] = cos(angle) * x - sin(angle) * y;
            triangle.vertices[i + 1] = sin(angle) * x + cos(angle) * y;
        }
        break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // ��и� ���м�
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    for (int i = 0; i < 4; ++i) {
        for (auto& triangle : triangles) {
            drawTriangle(triangle);
            updateTriangle(triangle);  // �ﰢ�� ������Ʈ (�̵�)
        }
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '4') {
        movePattern = key - '0';  // �̵� ���� ���� (1~4)
        for (int i = 0; i < 4; ++i) {
            for (auto& triangle : triangles) {
                triangle.moveType = movePattern;
            }
        }
    }

    glutPostRedisplay();
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);  // 16ms �Ŀ� �ٽ� ȣ�� (�� 60FPS)
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(0)));  // ���� �õ� ����

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Moving Triangles");

    initTriangle();
    glewInit();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);  // Ÿ�̸� ����

    glutMainLoop();
    return 0;
}

#endif