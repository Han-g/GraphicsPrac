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

GLuint shaderProgram;
bool wireframe = false;

struct Triangle {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> color;
};

std::vector<Triangle> triangles[4];  // 4���� ��и鿡 ���� �ﰢ�� ��� ����

int getQuadrant(float x, float y) {
    if (x > 0 && y > 0) return 0;  // 1��и�
    if (x < 0 && y > 0) return 1;  // 2��и�
    if (x < 0 && y < 0) return 2;  // 3��и�
    if (x > 0 && y < 0) return 3;  // 4��и�
    return -1;  // ȭ�� ��
}

void addTriangle(int quadrant, float x, float y) {
    if (triangles[quadrant].size() >= 3) return;  // �� ��и鿡 �ִ� 3���� �ﰢ���� ���

    GLfloat size = (rand() % 10 + 20) / 100.0f;  // �ﰢ�� ũ�� (����)
    GLfloat height = size;  // �ﰢ�� ����
    GLfloat base = size * 2.0f;  // �ﰢ�� �غ�

    // ���� ����
    GLfloat colorR = rand() % 256 / 255.0f;
    GLfloat colorG = rand() % 256 / 255.0f;
    GLfloat colorB = rand() % 256 / 255.0f;

    std::vector<GLfloat> vertices = {
        x, y - height,       // ������ 1 (�߾� �Ʒ�)
        x - base / 2, y + height,  // ������ 2 (���� ��)
        x + base / 2, y + height   // ������ 3 (������ ��)
    };

    std::vector<GLfloat> color = { colorR, colorG, colorB };

    Triangle triangle = { vertices, color };
    triangles[quadrant].push_back(triangle);  // �ﰢ�� �߰�
}

void drawTriangle(const Triangle& triangle) {
    if (wireframe)
        glBegin(GL_LINE_LOOP);  // ������ �׸���
    else
        glBegin(GL_TRIANGLES);  // ������ �׸���

    glColor3f(triangle.color[0], triangle.color[1], triangle.color[2]);
    glVertex2f(triangle.vertices[0], triangle.vertices[1]);  // ������ 1
    glVertex2f(triangle.vertices[2], triangle.vertices[3]);  // ������ 2
    glVertex2f(triangle.vertices[4], triangle.vertices[5]);  // ������ 3

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // ��и� ���м� �׸���
    glColor3f(1.0f, 1.0f, 1.0f);  // ���
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    // ��и��� �ﰢ�� �׸���
    for (int i = 0; i < 4; ++i) {
        for (auto& triangle : triangles[i]) {
            drawTriangle(triangle);
        }
    }

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
        float oglX = (2.0f * x / winWidth) - 1.0f;  // OpenGL ��ǥ�� ��ȯ
        float oglY = 1.0f - (2.0f * y / winHeight); // Y ��ǥ ��ȯ

        int quadrant = getQuadrant(oglX, oglY);

        if (button == GLUT_LEFT_BUTTON) {
            if (quadrant != -1) {
                triangles[quadrant].clear();  // �ش� ��и��� �ﰢ�� ����
                addTriangle(quadrant, oglX, oglY);  // ���ο� �ﰢ�� �߰�
            }
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            if (quadrant != -1 && triangles[quadrant].size() < 3) {
                addTriangle(quadrant, oglX, oglY);  // �ﰢ�� �߰�
            }
        }

        glutPostRedisplay();  // ȭ�� ����
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a') {
        wireframe = false;  // ������ �׸���
    }
    else if (key == 'b') {
        wireframe = true;   // ������ �׸���
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(0)));  // ���� ������ ���� �õ� ����

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Quadrant Triangles");

    glewInit();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}

#endif