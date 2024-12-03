#define ON 0

#if ON == 1

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <cmath>

// ��� ����
const float PI = 3.14159265358979323846f;

// ���� ������
bool isPerspective = true;          // ���� ���� ����
bool isWireframe = false;           // ���̾������� ��� ����
float posX = 0.0f, posY = 0.0f, posZ = -20.0f; // ��ü�� ��ġ
float rotateY = 0.0f;               // Y�� ȸ�� ����
float rotateZ = 0.0f;               // Z�� ȸ�� ����

float lightAngle = 0.0f;                  // ������ ȸ�� ����
GLfloat lightColor[3][4] = {              // ���� ���� �迭
    {1.0f, 1.0f, 1.0f, 1.0f},            // ���
    {1.0f, 0.0f, 0.0f, 1.0f},            // ������
    {0.0f, 0.0f, 1.0f, 1.0f}             // �Ķ���
};
int currentLightColor = 0;                // ���� ���� ���� �ε���

// �� �༺�� ���� ȸ�� ���� �� �ӵ�
float anglePlanet1 = 0.0f, speedPlanet1 = 0.6f;
float anglePlanet2 = 0.0f, speedPlanet2 = 0.4f;
float anglePlanet3 = 0.0f, speedPlanet3 = 0.2f;

float angleMoon1 = 0.0f, speedMoon1 = 1.2f;
float angleMoon2 = 0.0f, speedMoon2 = 0.8f;
float angleMoon3 = 0.0f, speedMoon3 = 0.4f;

// �Լ� ����
void init();
void display();
void reshape(int width, int height);
void idle();
void keyboard(unsigned char key, int x, int y);
void drawSphere(float radius);
void drawOrbit(float radius, float tilt);

// �ʱ�ȭ �Լ�
void init() {
    glEnable(GL_DEPTH_TEST); // ���� �׽�Ʈ Ȱ��ȭ
    glEnable(GL_LIGHTING);   // ���� Ȱ��ȭ
    glEnable(GL_LIGHT0);     // �⺻ ���� ���
    glEnable(GL_COLOR_MATERIAL); // ���� ���� Ȱ��ȭ
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ���� ������

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor[currentLightColor]);
}

// ���÷��� �ݹ� �Լ�
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ���� �ʱ�ȭ
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ī�޶� ��ġ ����
    glTranslatef(posX, posY, posZ);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);

    // �߾��� ��
    glColor3f(1.0f, 1.0f, 1.0f); // ���
    drawSphere(2.0f);

    // �˵� 1: xz ���
    glColor3f(0.5f, 0.5f, 0.5f); // ȸ��
    drawOrbit(6.0f, 0.0f);

    // �˵� 2: xz ��� 45�� ����
    drawOrbit(9.0f, 45.0f);

    // �˵� 3: xz ��� -45�� ����
    drawOrbit(12.0f, -45.0f);

    // �༺ 1
    glPushMatrix();
    float x1 = 6.0f * cos(anglePlanet1 * PI / 180.0f);
    float z1 = 6.0f * sin(anglePlanet1 * PI / 180.0f);
    glTranslatef(x1, 0.0f, z1);
    glColor3f(1.0f, 0.0f, 0.0f); // ������
    drawSphere(1.0f);

    // �� 1
    glPushMatrix();
    float xm1 = 2.0f * cos(angleMoon1 * PI / 180.0f);
    float zm1 = 2.0f * sin(angleMoon1 * PI / 180.0f);
    glTranslatef(xm1, 0.0f, zm1);
    glColor3f(0.5f, 0.5f, 0.5f); // ȸ��
    drawSphere(0.3f);
    glPopMatrix();
    glPopMatrix();

    // �༺ 2
    glPushMatrix();
    float angle2_rad = (anglePlanet2 * PI / 180.0f) + (45.0f * PI / 180.0f);
    float x2 = 9.0f * cos(angle2_rad);
    float y2 = 9.0f * sin(45.0f * PI / 180.0f) * sin(anglePlanet2 * PI / 180.0f);
    float z2 = 9.0f * sin(angle2_rad);
    glTranslatef(x2, y2, z2);
    glColor3f(0.0f, 1.0f, 0.0f); // �ʷϻ�
    drawSphere(1.0f);

    // �� 2
    glPushMatrix();
    float xm2 = 2.0f * cos(angleMoon2 * PI / 180.0f);
    float ym2 = 2.0f * sin(45.0f * PI / 180.0f) * sin(angleMoon2 * PI / 180.0f);
    float zm2 = 2.0f * sin(angleMoon2 * PI / 180.0f);
    glTranslatef(xm2, ym2, zm2);
    glColor3f(0.5f, 0.5f, 0.5f); // ȸ��
    drawSphere(0.3f);
    glPopMatrix();
    glPopMatrix();

    // �༺ 3
    glPushMatrix();
    float angle3_rad = (anglePlanet3 * PI / 180.0f) - (45.0f * PI / 180.0f);
    float x3 = 12.0f * cos(angle3_rad);
    float y3 = 12.0f * sin(-45.0f * PI / 180.0f) * sin(anglePlanet3 * PI / 180.0f);
    float z3 = 12.0f * sin(angle3_rad);
    glTranslatef(x3, y3, z3);
    glColor3f(0.0f, 0.0f, 1.0f); // �Ķ���
    drawSphere(1.0f);

    // �� 3
    glPushMatrix();
    float xm3 = 2.0f * cos(angleMoon3 * PI / 180.0f);
    float ym3 = 2.0f * sin(-45.0f * PI / 180.0f) * sin(angleMoon3 * PI / 180.0f);
    float zm3 = 2.0f * sin(angleMoon3 * PI / 180.0f);
    glTranslatef(xm3, ym3, zm3);
    glColor3f(0.5f, 0.5f, 0.5f); // ȸ��
    drawSphere(0.3f);
    glPopMatrix();
    glPopMatrix();

    // ���� ��ġ �� ǥ��
    GLfloat lightPosition[] = {
        10.0f * cos(lightAngle * PI / 180.0f), // ���� x ��ǥ
        0.0f,                                  // ���� y ��ǥ
        10.0f * sin(lightAngle * PI / 180.0f), // ���� z ��ǥ
        1.0f                                   // �� ����
    };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); // ���� ��ġ ����

    // ������ ��ġ�� ��Ÿ���� ��
    glPushMatrix();
    glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
    glColor3f(1.0f, 1.0f, 0.0f); // �����
    drawSphere(0.5f);
    glPopMatrix();

    glutSwapBuffers(); // ���� ��ü
}

// â ũ�� ���� �ݹ� �Լ�
void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (isPerspective) {
        gluPerspective(45.0f, aspect, 1.0f, 100.0f); // ���� ����
    }
    else {
        glOrtho(-20.0f * aspect, 20.0f * aspect, -20.0f, 20.0f, 1.0f, 100.0f); // ���� ����
    }

    glViewport(0, 0, width, height);
}

// ������Ʈ �Լ� (�ִϸ��̼�)
void idle() {
    // ���� ������Ʈ
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

    glutPostRedisplay(); // ȭ�� ���� ��û
}

// Ű���� �Է� ó�� �Լ�
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
    case 27: // ESC Ű
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// ���� �׸��� �Լ�
void drawSphere(float radius) {
    if (isWireframe)
        glutWireSphere(radius, 20, 20);
    else
        glutSolidSphere(radius, 20, 20);
}

// �˵��� �׸��� �Լ�
void drawOrbit(float radius, float tilt) {
    glPushMatrix();
    glRotatef(tilt, 1.0f, 0.0f, 0.0f); // �˵� ����
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = i * PI / 180.0f;
        glVertex3f(radius * cos(theta), 0.0f, radius * sin(theta));
    }
    glEnd();
    glPopMatrix();
}

// ���� �Լ�
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("�༺�� ���� �ִϸ��̼�");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        // GLEW �ʱ�ȭ ����
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }

    init();

    // �ݹ� �Լ� ���
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    glutMainLoop(); // GLUT ���� ���� ����
    return 0;
}

#endif