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

std::vector<Triangle> triangles[4];  // 4개의 사분면에 각각 삼각형 목록 저장

int getQuadrant(float x, float y) {
    if (x > 0 && y > 0) return 0;  // 1사분면
    if (x < 0 && y > 0) return 1;  // 2사분면
    if (x < 0 && y < 0) return 2;  // 3사분면
    if (x > 0 && y < 0) return 3;  // 4사분면
    return -1;  // 화면 밖
}

void addTriangle(int quadrant, float x, float y) {
    if (triangles[quadrant].size() >= 3) return;  // 각 사분면에 최대 3개의 삼각형만 허용

    GLfloat size = (rand() % 10 + 20) / 100.0f;  // 삼각형 크기 (랜덤)
    GLfloat height = size;  // 삼각형 높이
    GLfloat base = size * 2.0f;  // 삼각형 밑변

    // 랜덤 색상
    GLfloat colorR = rand() % 256 / 255.0f;
    GLfloat colorG = rand() % 256 / 255.0f;
    GLfloat colorB = rand() % 256 / 255.0f;

    std::vector<GLfloat> vertices = {
        x, y - height,       // 꼭짓점 1 (중앙 아래)
        x - base / 2, y + height,  // 꼭짓점 2 (왼쪽 위)
        x + base / 2, y + height   // 꼭짓점 3 (오른쪽 위)
    };

    std::vector<GLfloat> color = { colorR, colorG, colorB };

    Triangle triangle = { vertices, color };
    triangles[quadrant].push_back(triangle);  // 삼각형 추가
}

void drawTriangle(const Triangle& triangle) {
    if (wireframe)
        glBegin(GL_LINE_LOOP);  // 선으로 그리기
    else
        glBegin(GL_TRIANGLES);  // 면으로 그리기

    glColor3f(triangle.color[0], triangle.color[1], triangle.color[2]);
    glVertex2f(triangle.vertices[0], triangle.vertices[1]);  // 꼭짓점 1
    glVertex2f(triangle.vertices[2], triangle.vertices[3]);  // 꼭짓점 2
    glVertex2f(triangle.vertices[4], triangle.vertices[5]);  // 꼭짓점 3

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 사분면 구분선 그리기
    glColor3f(1.0f, 1.0f, 1.0f);  // 흰색
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    // 사분면의 삼각형 그리기
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
        float oglX = (2.0f * x / winWidth) - 1.0f;  // OpenGL 좌표로 변환
        float oglY = 1.0f - (2.0f * y / winHeight); // Y 좌표 변환

        int quadrant = getQuadrant(oglX, oglY);

        if (button == GLUT_LEFT_BUTTON) {
            if (quadrant != -1) {
                triangles[quadrant].clear();  // 해당 사분면의 삼각형 삭제
                addTriangle(quadrant, oglX, oglY);  // 새로운 삼각형 추가
            }
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            if (quadrant != -1 && triangles[quadrant].size() < 3) {
                addTriangle(quadrant, oglX, oglY);  // 삼각형 추가
            }
        }

        glutPostRedisplay();  // 화면 갱신
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a') {
        wireframe = false;  // 면으로 그리기
    }
    else if (key == 'b') {
        wireframe = true;   // 선으로 그리기
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(0)));  // 랜덤 색상을 위해 시드 설정

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