#define ON 0

#if ON == 1

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <cmath>

// 상수 정의
const float PI = 3.14159265358979323846f;

// 전역 변수들
bool isPerspective = true;          // 직각 투영 여부
bool isWireframe = false;           // 와이어프레임 모드 여부
float posX = 0.0f, posY = 0.0f, posZ = -20.0f; // 객체의 위치
float rotateY = 0.0f;               // Y축 회전 각도
float rotateZ = 0.0f;               // Z축 회전 각도

float lightAngle = 0.0f;                  // 조명의 회전 각도
GLfloat lightColor[3][4] = {              // 조명 색상 배열
    {1.0f, 1.0f, 1.0f, 1.0f},            // 흰색
    {1.0f, 0.0f, 0.0f, 1.0f},            // 빨간색
    {0.0f, 0.0f, 1.0f, 1.0f}             // 파란색
};
int currentLightColor = 0;                // 현재 조명 색상 인덱스

// 각 행성과 달의 회전 각도 및 속도
float anglePlanet1 = 0.0f, speedPlanet1 = 0.6f;
float anglePlanet2 = 0.0f, speedPlanet2 = 0.4f;
float anglePlanet3 = 0.0f, speedPlanet3 = 0.2f;

float angleMoon1 = 0.0f, speedMoon1 = 1.2f;
float angleMoon2 = 0.0f, speedMoon2 = 0.8f;
float angleMoon3 = 0.0f, speedMoon3 = 0.4f;

// 함수 선언
void init();
void display();
void reshape(int width, int height);
void idle();
void keyboard(unsigned char key, int x, int y);
void drawSphere(float radius);
void drawOrbit(float radius, float tilt);

// 초기화 함수
void init() {
    glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화
    glEnable(GL_LIGHTING);   // 조명 활성화
    glEnable(GL_LIGHT0);     // 기본 조명 사용
    glEnable(GL_COLOR_MATERIAL); // 색상 재질 활성화
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 배경색 검은색

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor[currentLightColor]);
}

// 디스플레이 콜백 함수
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 버퍼 초기화
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 카메라 위치 설정
    glTranslatef(posX, posY, posZ);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);

    // 중앙의 구
    glColor3f(1.0f, 1.0f, 1.0f); // 흰색
    drawSphere(2.0f);

    // 궤도 1: xz 평면
    glColor3f(0.5f, 0.5f, 0.5f); // 회색
    drawOrbit(6.0f, 0.0f);

    // 궤도 2: xz 평면 45도 기울기
    drawOrbit(9.0f, 45.0f);

    // 궤도 3: xz 평면 -45도 기울기
    drawOrbit(12.0f, -45.0f);

    // 행성 1
    glPushMatrix();
    float x1 = 6.0f * cos(anglePlanet1 * PI / 180.0f);
    float z1 = 6.0f * sin(anglePlanet1 * PI / 180.0f);
    glTranslatef(x1, 0.0f, z1);
    glColor3f(1.0f, 0.0f, 0.0f); // 빨간색
    drawSphere(1.0f);

    // 달 1
    glPushMatrix();
    float xm1 = 2.0f * cos(angleMoon1 * PI / 180.0f);
    float zm1 = 2.0f * sin(angleMoon1 * PI / 180.0f);
    glTranslatef(xm1, 0.0f, zm1);
    glColor3f(0.5f, 0.5f, 0.5f); // 회색
    drawSphere(0.3f);
    glPopMatrix();
    glPopMatrix();

    // 행성 2
    glPushMatrix();
    float angle2_rad = (anglePlanet2 * PI / 180.0f) + (45.0f * PI / 180.0f);
    float x2 = 9.0f * cos(angle2_rad);
    float y2 = 9.0f * sin(45.0f * PI / 180.0f) * sin(anglePlanet2 * PI / 180.0f);
    float z2 = 9.0f * sin(angle2_rad);
    glTranslatef(x2, y2, z2);
    glColor3f(0.0f, 1.0f, 0.0f); // 초록색
    drawSphere(1.0f);

    // 달 2
    glPushMatrix();
    float xm2 = 2.0f * cos(angleMoon2 * PI / 180.0f);
    float ym2 = 2.0f * sin(45.0f * PI / 180.0f) * sin(angleMoon2 * PI / 180.0f);
    float zm2 = 2.0f * sin(angleMoon2 * PI / 180.0f);
    glTranslatef(xm2, ym2, zm2);
    glColor3f(0.5f, 0.5f, 0.5f); // 회색
    drawSphere(0.3f);
    glPopMatrix();
    glPopMatrix();

    // 행성 3
    glPushMatrix();
    float angle3_rad = (anglePlanet3 * PI / 180.0f) - (45.0f * PI / 180.0f);
    float x3 = 12.0f * cos(angle3_rad);
    float y3 = 12.0f * sin(-45.0f * PI / 180.0f) * sin(anglePlanet3 * PI / 180.0f);
    float z3 = 12.0f * sin(angle3_rad);
    glTranslatef(x3, y3, z3);
    glColor3f(0.0f, 0.0f, 1.0f); // 파란색
    drawSphere(1.0f);

    // 달 3
    glPushMatrix();
    float xm3 = 2.0f * cos(angleMoon3 * PI / 180.0f);
    float ym3 = 2.0f * sin(-45.0f * PI / 180.0f) * sin(angleMoon3 * PI / 180.0f);
    float zm3 = 2.0f * sin(angleMoon3 * PI / 180.0f);
    glTranslatef(xm3, ym3, zm3);
    glColor3f(0.5f, 0.5f, 0.5f); // 회색
    drawSphere(0.3f);
    glPopMatrix();
    glPopMatrix();

    // 조명 위치 및 표시
    GLfloat lightPosition[] = {
        10.0f * cos(lightAngle * PI / 180.0f), // 조명 x 좌표
        0.0f,                                  // 조명 y 좌표
        10.0f * sin(lightAngle * PI / 180.0f), // 조명 z 좌표
        1.0f                                   // 점 광원
    };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); // 조명 위치 설정

    // 조명의 위치를 나타내는 구
    glPushMatrix();
    glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
    glColor3f(1.0f, 1.0f, 0.0f); // 노란색
    drawSphere(0.5f);
    glPopMatrix();

    glutSwapBuffers(); // 버퍼 교체
}

// 창 크기 변경 콜백 함수
void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (isPerspective) {
        gluPerspective(45.0f, aspect, 1.0f, 100.0f); // 원근 투영
    }
    else {
        glOrtho(-20.0f * aspect, 20.0f * aspect, -20.0f, 20.0f, 1.0f, 100.0f); // 직각 투영
    }

    glViewport(0, 0, width, height);
}

// 업데이트 함수 (애니메이션)
void idle() {
    // 각도 업데이트
    /*anglePlanet1 += speedPlanet1;
    if (anglePlanet1 >= 360.0f) anglePlanet1 -= 360.0f;

    anglePlanet2 += speedPlanet2;
    if (anglePlanet2 >= 360.0f) anglePlanet2 -= 360.0f;

    anglePlanet3 += speedPlanet3;
    if (anglePlanet3 >= 360.0f) anglePlanet3 -= 360.0f;

    angleMoon1 += speedMoon1;
    if (angleMoon1 >= 360.0f) angleMoon1 -= 360.0f;

    angleMoon2 += speedMoon2;
    if (angleMoon2 >= 360.0f) angleMoon2 -= 360.0f;

    angleMoon3 += speedMoon3;
    if (angleMoon3 >= 360.0f) angleMoon3 -= 360.0f;*/

    lightAngle += 0.5f;
    if (lightAngle >= 360.f) { lightAngle -= 360.f; }

    glutPostRedisplay(); // 화면 갱신 요청
}

// 키보드 입력 처리 함수
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'p':
    case 'P':
        isPerspective = !isPerspective;
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        break;
    case 'm':
    case 'M':
        isWireframe = !isWireframe;
        if (isWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glutPostRedisplay();
        break;
    case 'w':
        posY += 0.5f;
        break;
    case 's':
        posY -= 0.5f;
        break;
    case 'a':
        posX -= 0.5f;
        break;
    case 'd':
        posX += 0.5f;
        break;
    case '+':
        posZ += 0.5f;
        break;
    case '-':
        posZ -= 0.5f;
        break;
    case 'y':
        rotateY += 5.0f;
        break;
    case 'Y':
        rotateY -= 5.0f;
        break;
    case 'z':
        rotateZ += 5.0f;
        break;
    case 'Z':
        rotateZ -= 5.0f;
        break;
    case 27: // ESC 키
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// 구를 그리는 함수
void drawSphere(float radius) {
    if (isWireframe)
        glutWireSphere(radius, 20, 20);
    else
        glutSolidSphere(radius, 20, 20);
}

// 궤도를 그리는 함수
void drawOrbit(float radius, float tilt) {
    glPushMatrix();
    glRotatef(tilt, 1.0f, 0.0f, 0.0f); // 궤도 기울기
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = i * PI / 180.0f;
        glVertex3f(radius * cos(theta), 0.0f, radius * sin(theta));
    }
    glEnd();
    glPopMatrix();
}

// 메인 함수
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("행성과 달의 애니메이션");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        // GLEW 초기화 실패
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }

    init();

    // 콜백 함수 등록
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    glutMainLoop(); // GLUT 메인 루프 시작
    return 0;
}

#endif