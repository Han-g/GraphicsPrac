#define PROB 8

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#if PROB == 7

GLuint shaderProgram;
GLuint VAO, VBO;

enum type {
    P,
    LINE,
    TRIANGLE,
    RECTANGLE
};
struct diagram {
    type t;
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> color;
};

std::vector<diagram> diagrams;
type currentType = P;

char* filetobuf(const char* filePath) {
    FILE* fptr;
    long length;
    char* buf;

    // fopen_s 사용으로 변경
    if (fopen_s(&fptr, filePath, "rb") != 0) {
        std::cerr << "파일을 열 수 없습니다: " << filePath << std::endl;
        return nullptr;
    }

    fseek(fptr, 0, SEEK_END); // 파일 끝으로 이동
    length = ftell(fptr);      // 파일 크기 확인
    buf = (char*)malloc(length + 1); // 버퍼 할당
    fseek(fptr, 0, SEEK_SET);  // 파일 시작으로 이동
    fread(buf, length, 1, fptr); // 파일 내용을 버퍼에 읽기
    fclose(fptr);              // 파일 닫기
    buf[length] = '\0';        // 널 종료 추가

    return buf;
}

void initShaders() {
    // 버텍스와 프래그먼트 셰이더 컴파일 및 링크
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = filetobuf("vertex.glsl"); // vertex.glsl 파일 읽기
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSource = filetobuf("fragment.glsl"); // fragment.glsl 파일 읽기
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void initDiagrams() {
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,  // 왼쪽 아래
         0.5f, -0.5f, 0.0f,  // 오른쪽 아래
         0.0f,  0.5f, 0.0f   // 위쪽
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawDiagram(const diagram& d) {
    glColor3f(d.color[0], d.color[1], d.color[2]);
    switch (d.t) {
    case P:
        glBegin(GL_POINTS);
        glVertex3f(d.vertices[0], d.vertices[1], d.vertices[2]);
        glEnd();
        break;

    case LINE:
        glBegin(GL_LINES);
        glVertex3f(d.vertices[0], d.vertices[1], d.vertices[2]);
        glVertex3f(d.vertices[3], d.vertices[4], d.vertices[5]);
        glEnd();
        break;

    case TRIANGLE:
        glBegin(GL_TRIANGLES);
        glVertex3f(d.vertices[0], d.vertices[1], d.vertices[2]);
        glVertex3f(d.vertices[3], d.vertices[4], d.vertices[5]);
        glVertex3f(d.vertices[6], d.vertices[7], d.vertices[8]);
        glEnd();
        break;

    case RECTANGLE:
        glBegin(GL_QUADS);
        for (size_t i = 0; i < d.vertices.size(); i += 3) {
            glVertex3f(d.vertices[i], d.vertices[i + 1], d.vertices[i + 2]);
        }
        glEnd();
        break;
    }
}

void moveDiagram(diagram& d, int dir) {
    switch (d.t)
    {
    case P:
        if (dir == 1)       { d.vertices[1] += 0.1f; }
        else if (dir == 2)  { d.vertices[1] -= 0.1f; }
        else if (dir == 3)  { d.vertices[0] -= 0.1f; }
        else if (dir == 4)  { d.vertices[0] += 0.1f; }
        break;
    case LINE:
        if (dir == 1) {
            d.vertices[1] += 0.1f;
            d.vertices[4] += 0.1f;
        }
        else if (dir == 2) {
            d.vertices[1] -= 0.1f;
            d.vertices[4] -= 0.1f;
        }
        else if (dir == 3) {
            d.vertices[0] -= 0.1f;
            d.vertices[3] -= 0.1f;
        }
        else if (dir == 4) {
            d.vertices[0] += 0.1f;
            d.vertices[3] += 0.1f;
        }
        break;
    case TRIANGLE:
        for (int i = 0; i < 9; i += 3) {
            if (dir == 1)       { d.vertices[i + 1] += 0.1f; }
            else if (dir == 2)  { d.vertices[i + 1] -= 0.1f; }
            else if (dir == 3)  { d.vertices[i] -= 0.1f; }
            else if (dir == 4)  { d.vertices[i] += 0.1f; }
        }
        break;
    case RECTANGLE:
        for (int i = 0; i < 12; i += 3) {
            if (dir == 1)       { d.vertices[i + 1] += 0.1f; }
            else if (dir == 2)  { d.vertices[i + 1] -= 0.1f; }
            else if (dir == 3)  { d.vertices[i] -= 0.1f; }
            else if (dir == 4)  { d.vertices[i] += 0.1f; }
        }
        break;
    default:
        break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    for (const auto& d : diagrams) {
        drawDiagram(d);
    }
    glBindVertexArray(0);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    int select = 0;
    switch (key) {
    case 'p':
        currentType = P;
        break;
    case 'l':
        currentType = LINE;
        break;
    case 't':
        currentType = TRIANGLE;
        break;
    case 'r':
        currentType = RECTANGLE;
        break;
    case 'w':
        if (!diagrams.empty()) {
            select = rand() % diagrams.size();
            moveDiagram(diagrams[select], 1);
            glutPostRedisplay();
        }
        break;
    case 's':
        if (!diagrams.empty()) {
            select = rand() % diagrams.size();
            moveDiagram(diagrams[select], 2);
            glutPostRedisplay();
        }
        break;
    case 'a':
        if (!diagrams.empty()) {
            select = rand() % diagrams.size();
            moveDiagram(diagrams[select], 3);
            glutPostRedisplay();
        }
        break;
    case 'd':
        if (!diagrams.empty()) {
            select = rand() % diagrams.size();
            moveDiagram(diagrams[select], 4);
            glutPostRedisplay();
        }
        break;
    case 'c':
        diagrams.clear();
        glutPostRedisplay();
        break;
    case 'q':  // 프로그램 종료
        exit(0);
    }
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        diagram newDiagram;
        newDiagram.t = currentType;

        GLfloat mouseX = (x / 400.0f) - 1.0f;
        GLfloat mouseY = -((y / 300.0f) - 1.0f);

        newDiagram.color = {
            static_cast<GLfloat>(rand()%100) / 100.f, // R
            static_cast<GLfloat>(rand()%100) / 100.f, // G
            static_cast<GLfloat>(rand()%100) / 100.f  // B 
        };

        // 도형에 따라 꼭짓점 정보 설정
        switch (currentType) {
        case P:
            newDiagram.vertices = { mouseX, mouseY, 0.0f };
            break;
        case LINE:
            newDiagram.vertices = { mouseX - 0.1f, mouseY, 0.0f,  mouseX + 0.1f, mouseY, 0.0f };
            break;
        case TRIANGLE:
            newDiagram.vertices = {
                mouseX - 0.1f, mouseY - 0.1f, 0.0f,
                mouseX + 0.1f, mouseY - 0.1f, 0.0f,
                mouseX, mouseY + 0.1f, 0.0f
            };
            break;
        case RECTANGLE:
            newDiagram.vertices = { 
                mouseX - 0.1f, mouseY - 0.1f, 0.0f,
                mouseX + 0.1f, mouseY - 0.1f, 0.0f,
                mouseX + 0.1f, mouseY + 0.1f, 0.0f,
                mouseX - 0.1f, mouseY + 0.1f, 0.0f 
            };
            break;
        }

        if (diagrams.size() < 10) { diagrams.push_back(newDiagram); }
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Triangle");

    glewInit();

    initShaders();
    initDiagrams();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}

#elif PROB == 8



#endif