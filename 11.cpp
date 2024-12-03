#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <vector>

#define ON 0

#if ON == 1

// 도형의 상태를 저장하기 위한 구조체
struct Shape {
    std::vector<std::pair<float, float>> vertices;  // 꼭짓점 좌표
    int type;  // 도형 타입 (0: 선, 1: 삼각형, 2: 사각형, 3: 오각형)
    float t;   // 애니메이션을 위한 보간 값 (0.0 ~ 1.0)
};

std::vector<Shape> shapes(4);  // 4개의 도형을 저장
bool animate = false;  // 애니메이션 진행 여부

// 사각형 변환을 위한 함수
void interpolateShape(Shape& shape, int targetType, float t) {
    if (shape.type == 0 && targetType == 1) {  // 선 -> 삼각형
        // 선에서 삼각형으로 변환
        if (shape.vertices.size() == 2) {
            shape.vertices.push_back({ 0.0f, 0.5f });  // 삼각형의 세 번째 꼭짓점 추가
        }
    }
    else if (shape.type == 1 && targetType == 2) {  // 삼각형 -> 사각형
        // 삼각형에서 사각형으로 변환
        if (shape.vertices.size() == 3) {
            shape.vertices.push_back({ 0.5f, -0.5f });  // 사각형의 네 번째 꼭짓점 추가
        }
    }
    else if (shape.type == 2 && targetType == 3) {  // 사각형 -> 오각형
        // 사각형에서 오각형으로 변환
        if (shape.vertices.size() == 4) {
            shape.vertices.push_back({ 0.0f, 0.6f });  // 오각형의 다섯 번째 꼭짓점 추가
        }
    }
    else if (shape.type == 3 && targetType == 0) {  // 오각형 -> 선
        // 오각형에서 선으로 변환 (역변환)
        shape.vertices.resize(2);  // 선의 두 꼭짓점만 남김
    }

    // 도형 타입을 목표 타입으로 변경
    shape.type = targetType;
    shape.t = 0.0f;  // 애니메이션 보간 값 초기화
}

// 도형을 그리는 함수
void drawShape(const Shape& shape) {
    if (shape.vertices.empty()) return;

    // 선 또는 다각형 그리기
    if (shape.type == 0) glBegin(GL_LINES);  // 선
    else glBegin(GL_POLYGON);  // 다각형

    for (const auto& vertex : shape.vertices) {
        glVertex2f(vertex.first, vertex.second);
    }

    glEnd();
}

// 각 사분면에 도형 그리기
void drawShapes() {
    // 화면을 4분면으로 나눠 각 사분면에 도형을 그리기
    glPushMatrix();
    glTranslatef(-0.5f, 0.5f, 0.0f);  // 1사분면
    drawShape(shapes[0]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, 0.5f, 0.0f);  // 2사분면
    drawShape(shapes[1]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5f, -0.5f, 0.0f);  // 3사분면
    drawShape(shapes[2]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, -0.5f, 0.0f);  // 4사분면
    drawShape(shapes[3]);
    glPopMatrix();
}

// 초기 도형 설정
void initializeShapes() {
    // 각 사분면에 초기 도형을 설정
    shapes[0] = { { {-0.5f, 0.0f}, {0.5f, 0.0f} }, 0, 0.0f };  // 선
    shapes[1] = { { {0.0f, 0.5f}, {-0.5f, -0.5f}, {0.5f, -0.5f} }, 1, 0.0f };  // 삼각형
    shapes[2] = { { {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.5f, -0.5f}, {-0.5f, -0.5f} }, 2, 0.0f };  // 사각형
    shapes[3] = { { {0.0f, 0.5f}, {0.5f, 0.0f}, {0.3f, -0.5f}, {-0.3f, -0.5f}, {-0.5f, 0.0f} }, 3, 0.0f };  // 오각형
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 도형 그리기
    drawShapes();

    glutSwapBuffers();
}

void timer(int value) {
    if (animate) {
        for (auto& shape : shapes) {
            if (shape.t < 1.0f) {
                shape.t += 0.01f;  // 애니메이션 속도
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// 키보드 입력 처리
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'l':  // 선 -> 삼각형
        for (auto& shape : shapes) {
            if (shape.type == 0) interpolateShape(shape, 1, shape.t);
        }
        break;
    case 't':  // 삼각형 -> 사각형
        for (auto& shape : shapes) {
            if (shape.type == 1) interpolateShape(shape, 2, shape.t);
        }
        break;
    case 'r':  // 사각형 -> 오각형
        for (auto& shape : shapes) {
            if (shape.type == 2) interpolateShape(shape, 3, shape.t);
        }
        break;
    case 'p':  // 오각형 -> 선
        for (auto& shape : shapes) {
            if (shape.type == 3) interpolateShape(shape, 0, shape.t);
        }
        break;
    case 'a':  // 네 개의 도형 모두 초기화
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
    glutTimerFunc(0, timer, 0);  // 애니메이션을 위한 타이머

    glutMainLoop();
    return 0;
}

#endif