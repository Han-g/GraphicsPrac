#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <vector>

#define ON 0

#if ON == 1

// ������ ���¸� �����ϱ� ���� ����ü
struct Shape {
    std::vector<std::pair<float, float>> vertices;  // ������ ��ǥ
    int type;  // ���� Ÿ�� (0: ��, 1: �ﰢ��, 2: �簢��, 3: ������)
    float t;   // �ִϸ��̼��� ���� ���� �� (0.0 ~ 1.0)
};

std::vector<Shape> shapes(4);  // 4���� ������ ����
bool animate = false;  // �ִϸ��̼� ���� ����

// �簢�� ��ȯ�� ���� �Լ�
void interpolateShape(Shape& shape, int targetType, float t) {
    if (shape.type == 0 && targetType == 1) {  // �� -> �ﰢ��
        // ������ �ﰢ������ ��ȯ
        if (shape.vertices.size() == 2) {
            shape.vertices.push_back({ 0.0f, 0.5f });  // �ﰢ���� �� ��° ������ �߰�
        }
    }
    else if (shape.type == 1 && targetType == 2) {  // �ﰢ�� -> �簢��
        // �ﰢ������ �簢������ ��ȯ
        if (shape.vertices.size() == 3) {
            shape.vertices.push_back({ 0.5f, -0.5f });  // �簢���� �� ��° ������ �߰�
        }
    }
    else if (shape.type == 2 && targetType == 3) {  // �簢�� -> ������
        // �簢������ ���������� ��ȯ
        if (shape.vertices.size() == 4) {
            shape.vertices.push_back({ 0.0f, 0.6f });  // �������� �ټ� ��° ������ �߰�
        }
    }
    else if (shape.type == 3 && targetType == 0) {  // ������ -> ��
        // ���������� ������ ��ȯ (����ȯ)
        shape.vertices.resize(2);  // ���� �� �������� ����
    }

    // ���� Ÿ���� ��ǥ Ÿ������ ����
    shape.type = targetType;
    shape.t = 0.0f;  // �ִϸ��̼� ���� �� �ʱ�ȭ
}

// ������ �׸��� �Լ�
void drawShape(const Shape& shape) {
    if (shape.vertices.empty()) return;

    // �� �Ǵ� �ٰ��� �׸���
    if (shape.type == 0) glBegin(GL_LINES);  // ��
    else glBegin(GL_POLYGON);  // �ٰ���

    for (const auto& vertex : shape.vertices) {
        glVertex2f(vertex.first, vertex.second);
    }

    glEnd();
}

// �� ��и鿡 ���� �׸���
void drawShapes() {
    // ȭ���� 4�и����� ���� �� ��и鿡 ������ �׸���
    glPushMatrix();
    glTranslatef(-0.5f, 0.5f, 0.0f);  // 1��и�
    drawShape(shapes[0]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, 0.5f, 0.0f);  // 2��и�
    drawShape(shapes[1]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5f, -0.5f, 0.0f);  // 3��и�
    drawShape(shapes[2]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, -0.5f, 0.0f);  // 4��и�
    drawShape(shapes[3]);
    glPopMatrix();
}

// �ʱ� ���� ����
void initializeShapes() {
    // �� ��и鿡 �ʱ� ������ ����
    shapes[0] = { { {-0.5f, 0.0f}, {0.5f, 0.0f} }, 0, 0.0f };  // ��
    shapes[1] = { { {0.0f, 0.5f}, {-0.5f, -0.5f}, {0.5f, -0.5f} }, 1, 0.0f };  // �ﰢ��
    shapes[2] = { { {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.5f, -0.5f}, {-0.5f, -0.5f} }, 2, 0.0f };  // �簢��
    shapes[3] = { { {0.0f, 0.5f}, {0.5f, 0.0f}, {0.3f, -0.5f}, {-0.3f, -0.5f}, {-0.5f, 0.0f} }, 3, 0.0f };  // ������
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // ���� �׸���
    drawShapes();

    glutSwapBuffers();
}

void timer(int value) {
    if (animate) {
        for (auto& shape : shapes) {
            if (shape.t < 1.0f) {
                shape.t += 0.01f;  // �ִϸ��̼� �ӵ�
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// Ű���� �Է� ó��
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'l':  // �� -> �ﰢ��
        for (auto& shape : shapes) {
            if (shape.type == 0) interpolateShape(shape, 1, shape.t);
        }
        break;
    case 't':  // �ﰢ�� -> �簢��
        for (auto& shape : shapes) {
            if (shape.type == 1) interpolateShape(shape, 2, shape.t);
        }
        break;
    case 'r':  // �簢�� -> ������
        for (auto& shape : shapes) {
            if (shape.type == 2) interpolateShape(shape, 3, shape.t);
        }
        break;
    case 'p':  // ������ -> ��
        for (auto& shape : shapes) {
            if (shape.type == 3) interpolateShape(shape, 0, shape.t);
        }
        break;
    case 'a':  // �� ���� ���� ��� �ʱ�ȭ
        initializeShapes();
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shape Transformation Animation");

    initializeShapes();
    glewInit();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);  // �ִϸ��̼��� ���� Ÿ�̸�

    glutMainLoop();
    return 0;
}

#endif