#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

#define ON 0

#if ON == 1

struct Spiral {
    float x, y;          // �߽� ��ǥ
    bool clockwise;      // �ð� ���� �Ǵ� �ݽð� ����
    int numTurns;        // ȸ�� ��
    bool inward;         // �ȿ��� ������ or �ۿ��� ������
    std::vector<std::pair<float, float>> points;  // �����̷��� ����
    bool complete;       // �ִϸ��̼� �Ϸ� ����
};

std::vector<Spiral> spirals;  // �׸� �����̷���
int currentNumSpirals = 1;    // �׸� �����̷��� ��
bool drawAsPoints = false;    // ������ �׸��� ����
float bgColor[3] = { 0.0f, 0.0f, 0.0f };  // ����

void changeBackgroundColor() {
    // ������ �����ϰ� ����
    bgColor[0] = static_cast<float>(rand()) / RAND_MAX;
    bgColor[1] = static_cast<float>(rand()) / RAND_MAX;
    bgColor[2] = static_cast<float>(rand()) / RAND_MAX;
}

void generateSpiral(float x, float y, int numTurns, bool clockwise, bool inward) {
    Spiral spiral;
    spiral.x = x;
    spiral.y = y;
    spiral.clockwise = clockwise;
    spiral.numTurns = numTurns;
    spiral.inward = inward;
    spiral.complete = false;

    float angle = 0.0f;
    float radius = inward ? 0.05f : 0.001f;  // �ۿ��� �����δ� ū �ݰ���� ����
    float angleStep = clockwise ? 0.1f : -0.1f;
    float radiusStep = inward ? -0.005f : 0.005f;

    // �����̷��� �� ���
    for (int i = 0; i < numTurns * 60; ++i) {
        float px = spiral.x + radius * cos(angle);
        float py = spiral.y + radius * sin(angle);
        spiral.points.push_back({ px, py });

        angle += angleStep;
        radius += radiusStep;
    }

    spirals.push_back(spiral);
}

void drawSpiral(const Spiral& spiral) {
    glBegin(drawAsPoints ? GL_POINTS : GL_LINE_STRIP);
    for (const auto& point : spiral.points) {
        glVertex2f(point.first, point.second);
    }
    glEnd();
}

void display() {
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& spiral : spirals) {
        drawSpiral(spiral);
    }

    glutSwapBuffers();
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);  // 60 FPS�� �ִϸ��̼�
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        float mouseX = (2.0f * x / windowWidth) - 1.0f;
        float mouseY = 1.0f - (2.0f * y / windowHeight);

        changeBackgroundColor();  // ���Ӱ� �׸� ������ ���� ����

        for (int i = 0; i < currentNumSpirals; ++i) {
            bool clockwise = rand() % 2 == 0;  // �ð�/�ݽð� ���� ����
            bool inward = rand() % 2 == 0;     // �ȿ��� ������ or �ۿ��� ������ ����
            if (i == 0) {
                generateSpiral(mouseX, mouseY, 2, clockwise, inward);  // Ŭ�� ��ġ�� �����̷� ����
            }
            else {
                // ���� ��ġ�� �����̷� ����
                float randX = (rand() % 200 - 100) / 100.0f;
                float randY = (rand() % 200 - 100) / 100.0f;
                generateSpiral(randX, randY, 2, clockwise, inward);
            }
        }

        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'p':
        drawAsPoints = true;
        break;
    case 'l':
        drawAsPoints = false;
        break;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
        currentNumSpirals = key - '0';
        break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(0)));  // ���� �õ� ����

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Spiral Animation");

    glewInit();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);  // Ÿ�̸� ����

    glutMainLoop();
    return 0;
}

#endif